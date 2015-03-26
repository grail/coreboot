/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2009 One Laptop per Child, Association, Inc.
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

#include <arch/pirq_routing.h>

static const struct irq_routing_table intel_irq_routing_table = {
	PIRQ_SIGNATURE,		/* u32 signature */
	PIRQ_VERSION,		/* u16 version   */
	32 + 16 * CONFIG_IRQ_SLOT_COUNT,/* Max. number of devices on the bus */
	0x00,			/* Where the interrupt router lies (bus) */
	(0x11 << 3) | 0x0,	/* Where the interrupt router lies (dev) */
	0xca0,			/* IRQs devoted exclusively to PCI usage */
	0x1106,			/* Vendor */
	0x596,			/* Device */
	0,			/* Miniport */
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* u8 rfu[11] */
	0xdb,			/* Checksum. 0xa0? */
	{
		/* bus,        dev | fn,   {link, bitmap}, {link, bitmap}, {link, bitmap}, {link, bitmap},  slot, rfu */
		{0x00, (0x02 << 3) | 0x0, {{0x09, 0xdeb8}, {0x09, 0xdeb8}, {0x09, 0xdeb8}, {0x09, 0xdeb8}}, 0x1, 0x0},
		{0x00, (0x03 << 3) | 0x0, {{0x09, 0xdeb8}, {0x09, 0xdeb8}, {0x09, 0xdeb8}, {0x09, 0xdeb8}}, 0x2, 0x0},
		{0x00, (0x03 << 3) | 0x1, {{0x09, 0xdeb8}, {0x09, 0xdeb8}, {0x09, 0xdeb8}, {0x09, 0xdeb8}}, 0x3, 0x0},
		{0x04, (0x04 << 3) | 0x0, {{0x02, 0xdeb8}, {0x03, 0xdeb8}, {0x05, 0xdeb8}, {0x01, 0xdeb8}}, 0x4, 0x0},
		{0x04, (0x0e << 3) | 0x0, {{0x01, 0xdeb8}, {0x02, 0xdeb8}, {0x03, 0xdeb8}, {0x05, 0xdeb8}}, 0x5, 0x0},
		{0x00, (0x11 << 3) | 0x0, {{0x00, 0xdeb8}, {0x00, 0xdeb8}, {0x03, 0xdeb8}, {0x05, 0xdeb8}}, 0x0, 0x0},
		{0x00, (0x0f << 3) | 0x0, {{0x01, 0xdeb8}, {0x02, 0xdeb8}, {0x03, 0xdeb8}, {0x05, 0xdeb8}}, 0x0, 0x0},
		{0x00, (0x01 << 3) | 0x0, {{0x01, 0xdeb8}, {0x02, 0xdeb8}, {0x03, 0xdeb8}, {0x05, 0xdeb8}}, 0x0, 0x0},
		{0x00, (0x10 << 3) | 0x0, {{0x01, 0xdeb8}, {0x02, 0xdeb8}, {0x03, 0xdeb8}, {0x05, 0xdeb8}}, 0x0, 0x0},
		{0x00, (0x02 << 3) | 0x0, {{0x09, 0xdeb8}, {0x09, 0xdeb8}, {0x09, 0xdeb8}, {0x09, 0xdeb8}}, 0x0, 0x0},
		{0x00, (0x03 << 3) | 0x0, {{0x09, 0xdeb8}, {0x09, 0xdeb8}, {0x09, 0xdeb8}, {0x09, 0xdeb8}}, 0x0, 0x0},
		{0x00, (0x03 << 3) | 0x1, {{0x09, 0xdeb8}, {0x09, 0xdeb8}, {0x09, 0xdeb8}, {0x09, 0xdeb8}}, 0x0, 0x0},
		{0x00, (0x14 << 3) | 0x0, {{0x01, 0xdeb8}, {0x02, 0xdeb8}, {0x03, 0xdeb8}, {0x05, 0xdeb8}}, 0x0, 0x0},
	}
};

inline unsigned long write_pirq_routing_table(unsigned long addr)
{
	return copy_pirq_routing_table(addr, &intel_irq_routing_table);
}
