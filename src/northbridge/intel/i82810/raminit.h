/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2007 Corey Osgood <corey.osgood@gmail.com>
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

#ifndef NORTHBRIDGE_INTEL_I82810_RAMINIT_H
#define NORTHBRIDGE_INTEL_I82810_RAMINIT_H

/* The 82810 supports max. 2 dual-sided DIMMs. */
#define DIMM_SOCKETS	2

/* Function prototypes. */
void sdram_set_registers(void);
void sdram_set_spd_registers(void);
void sdram_enable(void);

/* Debug */
#if CONFIG_DEBUG_RAM_SETUP
void dump_spd_registers(void);
void dump_pci_device(unsigned dev);
#else
#define dump_spd_registers()
#endif
#endif
