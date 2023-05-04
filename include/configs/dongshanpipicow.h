/* SPDX-License-Identifier: GPL-2.0+ */
/*
 */

#ifndef __PREDITA_CONFIG_H
#define __PREDITA_CONFIG_H
#include "mstarv7.h"

#ifdef CONFIG_MSTAR_USBUPDATER
#define DONGSHANPIPICOW_DFU_ALT_NAND \
	"dfu_alt_info=all raw 0x0 0x8000000;"\
	"gcis raw 0x0 0x20000;"\
	"spl0 raw 0x20000 0x20000;"\
	"spl1 raw 0x40000 0x20000;"\
	"ubi raw 0x60000 0x7FA0000\0"

#define DONGSNANPIPICOW_DFU_START_NAND \
	"dongshanpipicow_start_dfu_nand=dfu 0 mtd spi-nand0\0"
	//"dongshanpipicow_start_dfu_nand=setenv dfu_alt_info ${dfu_alt_info_nand}; dfu 0 mtd spi-nand0\0"

#define DONGSHANPIPICOW_DFU_ALT_RAM \
	"dfu_alt_info_ram=image ram 24000000 1000000\0" \
	"dongshanpipicow_start_dfu_ram=setenv dfu_alt_info ${dfu_alt_info_ram}; dfu 0 ram 0\0"

//dfu 0 ram ram

#define DONGSHANPIPICOW_UBISETUP \
	"dongshanpipicow_createubiparts=ubi createvol env 0x20000 static;"\
	"ubi createvol kernel 0x800000 static;"\
	"ubi createvol root 0x2000000 static;"\
	"ubi createvol rescue 0x1000000 static\0"

#define DONGSHANPIPICO_UPDATER_OPTS \
	DONGSHANPIPICOW_DFU_ALT_NAND \
	DONGSNANPIPICOW_DFU_START_NAND \
	DONGSHANPIPICOW_DFU_ALT_RAM \
	DONGSHANPIPICOW_UBISETUP

#else
#define DONGSHANPIPICO_UPDATER_OPTS
#endif

#define CONFIG_EXTRA_ENV_SETTINGS "bootargs=" MSTARV7_CMDLINEOPTS "\0" \
	DONGSHANPIPICO_UPDATER_OPTS

#endif
