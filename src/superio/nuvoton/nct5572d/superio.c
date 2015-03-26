/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2014 Felix Held <felix-coreboot@felixheld.de>
 * Copyright (C) 2014 Edward O'Callaghan <eocallaghan@alterapraxis.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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

#include <arch/io.h>
#include <device/device.h>
#include <device/pnp.h>
#include <pc80/keyboard.h>
#include <stdlib.h>
#include <superio/conf_mode.h>

#include "nct5572d.h"

static void nct5572d_init(struct device *dev)
{
	if (!dev->enabled)
		return;

	switch(dev->path.pnp.device) {
	/* TODO: Might potentially need code for HWM or FDC etc. */
	case NCT5572D_KBC:
		pc_keyboard_init();
		break;
	}
}

static struct device_operations ops = {
	.read_resources   = pnp_read_resources,
	.set_resources    = pnp_set_resources,
	.enable_resources = pnp_enable_resources,
	.enable           = pnp_alt_enable,
	.init             = nct5572d_init,
	.ops_pnp_mode     = &pnp_conf_mode_8787_aa,
};

static struct pnp_info pnp_dev_info[] = {
	{ &ops, NCT5572D_SP1, PNP_IO0 | PNP_IRQ0, {0x0FF8, 0}, },
	{ &ops, NCT5572D_IR, PNP_IO0 | PNP_IRQ0, {0x0FF8, 0}, },
	{ &ops, NCT5572D_KBC, PNP_IO0 | PNP_IO1 | PNP_IRQ0 | PNP_IRQ1, {0x0FFF, 0}, {0x0FFF, 4}, },
	{ &ops, NCT5572D_CIR, PNP_IO0 | PNP_IRQ0, {0x0FF8, 0}, },
	{ &ops, NCT5572D_WDT1},
	{ &ops, NCT5572D_ACPI},
	{ &ops, NCT5572D_HWM_TSI_FPLED, PNP_IO0 | PNP_IO1 | PNP_IRQ0, {0x0FFE, 0}, {0x0FFE, 4}, },
	{ &ops, NCT5572D_PECI},
	{ &ops, NCT5572D_SUSLED},
	{ &ops, NCT5572D_CIRWKUP, PNP_IO0 | PNP_IRQ0, {0x0FF8, 0}, },
	{ &ops, NCT5572D_GPIO_PP_OD},
	{ &ops, NCT5572D_GPIO2},
	{ &ops, NCT5572D_GPIO3},
	{ &ops, NCT5572D_GPIO5},
	{ &ops, NCT5572D_GPIO6},
	{ &ops, NCT5572D_GPIO8},
	{ &ops, NCT5572D_GPIO9},
};

static void enable_dev(struct device *dev)
{
	pnp_enable_devices(dev, &ops, ARRAY_SIZE(pnp_dev_info), pnp_dev_info);
}

struct chip_operations superio_nuvoton_nct5572d_ops = {
	CHIP_NAME("NUVOTON NCT5572D Super I/O")
	.enable_dev = enable_dev,
};
