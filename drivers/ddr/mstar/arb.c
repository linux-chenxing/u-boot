/*
 * arb.c
 *
 *  Created on: 19 Aug 2022
 *      Author: daniel
 */

#include <chenxingv7.h>

#include "arb.h"

/* not sure what this does */
/* in p3 ipl */
void mstar_arb_mystery(void)
{
	mstar_writew(0x0000, MIU_ARB + MIU_ARB_C0);
	mstar_writew(0x0000, MIU_ARB + MIU_ARB_C4);
	mstar_writew(0x0000, MIU_ARB + MIU_ARB_C8);
	mstar_writew(0x0030, MIU_ARB + MIU_ARB_CC);
	mstar_writew(0x5000, MIU_ARB + MIU_ARB_D0);
}
