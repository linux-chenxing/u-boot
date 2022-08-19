/*
 * arb.h
 */

#ifndef DRIVERS_DDR_MSTAR_ARB_H_
#define DRIVERS_DDR_MSTAR_ARB_H_

#define MIU_ARB				0x1f202200
#define MIU_ARB_GROUP4_CTRL		0x00
#define MIU_ARB_GROUP4_REQ_MASK	0x0c
#define MIU_ARB_GROUP4_REQ_DEADLINE	0x24
#define MIU_ARB_GROUP5_CTRL		0x40
#define MIU_ARB_GROUP5_REQ_MASK	0x4c
#define MIU_ARB_GROUP5_REQ_DEADLINE	0x6c
#define MIU_ARB_C0			0xc0
#define MIU_ARB_C4			0xc4
#define MIU_ARB_C8			0xc8
#define MIU_ARB_CC			0xcc
#define MIU_ARB_D0			0xd0
#define MIU_ARB_PROTECT4_ID		0x1b8
#define MIU_ARB_PROTECT4_CTRL		0x1bc
#define MIU_ARB_1D0			0x1d0
#define MIU_ARB_1D4			0x1d4
#define MIU_ARB_1D8			0x1d8
// not sure
#define MIU_ARB_GROUP6_CTRL		0x1c0
#define MIU_ARB_GROUP6_REQ_MASK	0x1cc

void mstar_arb_mystery(struct regmap *arb);

#endif /* DRIVERS_DDR_MSTAR_ARB_H_ */
