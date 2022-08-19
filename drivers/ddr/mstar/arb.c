/*
 * arb.c
 *
 *  Created on: 19 Aug 2022
 *      Author: daniel
 */

#include <common.h>
#include <chenxingv7.h>
#include <dm.h>
#include <regmap.h>

#include "arb.h"
#include "debug.h"

/* not sure what this does */
/* in p3 ipl */
void mstar_arb_mystery(struct regmap *arb)
{
	mstar_regmap_write(arb, MIU_ARB_C0, 0x0000);
	mstar_regmap_write(arb, MIU_ARB_C4, 0x0000);
	mstar_regmap_write(arb, MIU_ARB_C8, 0x0000);
	mstar_regmap_write(arb, MIU_ARB_CC, 0x0030);
	mstar_regmap_write(arb, MIU_ARB_D0, 0x5000);
}
