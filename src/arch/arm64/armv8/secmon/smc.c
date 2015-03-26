/*
 * This file is part of the coreboot project.
 *
 * Copyright 2014 Google Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.
 */

#include <string.h>
#include <stdlib.h>
#include <arch/cpu.h>
#include <arch/smc.h>
#include <arch/exception.h>
#include <arch/lib_helpers.h>
#include <console/console.h>

enum {
	/* SMC called from AARCH32 */
	EC_SMC_AARCH32 = 0x13,
	/* SMC called from AARCH64 */
	EC_SMC_AARCH64 = 0x17,

	SMC_NUM_RANGES = 8,
};

struct smc_range {
	uint32_t func_begin;
	uint32_t func_end;
	int (*handler)(struct smc_call *);
};

struct smc_ranges {
	size_t used;
	struct smc_range handlers[SMC_NUM_RANGES];
};

static struct smc_ranges smc_functions;

static struct smc_range *smc_handler_by_function(uint32_t fid)
{
	int i;

	for (i = 0; i < smc_functions.used; i++) {
		struct smc_range *r = &smc_functions.handlers[i];

		if (fid >= r->func_begin && fid <= r->func_end)
			return r;
	}

	return NULL;
}

int smc_register_range(uint32_t min, uint32_t max, int (*h)(struct smc_call *))
{
	struct smc_range *r;

	if (smc_functions.used == SMC_NUM_RANGES)
		return -1;

	if (min > max)
		return -1;

	/* This check isn't exhaustive but it's fairly quick. */
	if (smc_handler_by_function(min) || smc_handler_by_function(max))
		return -1;

	r = &smc_functions.handlers[smc_functions.used];
	r->func_begin = min;
	r->func_end = max;
	r->handler = h;
	smc_functions.used++;

	return 0;
}

static int smc_cleanup(struct exc_state *state, struct smc_call *smc, int ret)
{
	memcpy(&state->regs.x, &smc->results, sizeof(smc->results));

	return ret;
}

static int smc_return_with_error(struct exc_state *state, struct smc_call *smc)
{
	smc32_return(smc, SMC_UNKNOWN_FUNC);
	return smc_cleanup(state, smc, EXC_RET_HANDLED);
}

static int smc_handler(struct exc_state *state, uint64_t vector_id)
{
	struct smc_call smc_storage;
	struct smc_call *smc = &smc_storage;
	uint32_t exception_class;
	uint32_t esr;
	struct smc_range *r;

	memcpy(&smc->args, &state->regs.x, sizeof(smc->args));
	memcpy(&smc->results, &state->regs.x, sizeof(smc->results));

	esr = raw_read_esr_el3();
	exception_class = (esr >> 26) & 0x3f;

	/* No support for SMC calls from AARCH32 */
	if (exception_class == EC_SMC_AARCH32)
		return smc_return_with_error(state, smc);

	/* Check to ensure this is an SMC from AARCH64. */
	if (exception_class != EC_SMC_AARCH64)
		return EXC_RET_IGNORED;

	/* Ensure immediate value is 0. */
	if ((esr & 0xffff) != 0)
		return smc_return_with_error(state, smc);

	r = smc_handler_by_function(smc_function_id(smc));

	if (r != NULL) {
		if (!r->handler(smc))
			return smc_cleanup(state, smc, EXC_RET_HANDLED);
	}

	return smc_return_with_error(state, smc);
}

/* SMC calls can be generated by 32-bit or 64-bit code. */
static struct exception_handler smc_handler64 = {
	.handler = &smc_handler,
};

static struct exception_handler smc_handler32 = {
	.handler = &smc_handler,
};

static void enable_smc(void *arg)
{
	uint32_t scr;

	/* Enable SMC */
	scr = raw_read_scr_el3();
	scr &= ~(SCR_SMC_MASK);
	scr |= SCR_SMC_ENABLE;
	raw_write_scr_el3(scr);
}

void smc_init(void)
{
	struct cpu_action action = {
		.run = enable_smc,
	};

	arch_run_on_all_cpus_async(&action);

	/* Register SMC handlers. */
	exception_handler_register(EXC_VID_LOW64_SYNC, &smc_handler64);
	exception_handler_register(EXC_VID_LOW32_SYNC, &smc_handler32);
}
