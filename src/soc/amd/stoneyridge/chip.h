/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2010-2017 Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef STONEYRIDGE_CHIP_H
#define STONEYRIDGE_CHIP_H

#include <stdint.h>

struct soc_amd_stoneyridge_config {
	u8 spdAddrLookup[1][1][2];
	u32 ide0_enable : 1;
	u32 sata0_enable : 1;
	u32 boot_switch_sata_ide : 1;
	u32 hda_viddid;
	u8  gpp_configuration;
	u8  sd_mode;
};

typedef struct soc_amd_stoneyridge_config config_t;

extern struct device_operations pci_domain_ops;

#endif /* STONEYRIDGE_CHIP_H */
