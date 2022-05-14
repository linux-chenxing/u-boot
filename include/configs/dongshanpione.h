/* SPDX-License-Identifier: GPL-2.0+ */
/*
 */

#ifndef __DONGSHANPIONE_CONFIG_H
#define __DONGSHANPIONE_CONFIG_H

#include "mstarv7.h"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"bootargs=console=ttyS0,115200 "\
	"ubi.fm_autoconvert=1 ubi.mtd=1 ubi.block=0,rootfs root=/dev/ubiblock0_4 "\
	MSTARV7_CMDLINEOPTS"\0"\
	"bootcmd_rescue=ubi readvol ${loadaddr} rescue; bootm ${loadaddr}\0"\
	"bootcmd=ubi readvol ${loadaddr} kernel; bootm ${loadaddr}\0"

#endif
