/*
 *
 */

#ifndef MSTAR_MMU_H_
#define MSTAR_MMU_H_

#define	MSTAR_MMU	0x1f202600

#define MSTAR_MMU_REG_44	0x44
#define MSTAR_MMU_REG_7C	0x7C
#define MSTAR_MMU_REG_80	0x80
#define MSTAR_MMU_REG_84	0x84
#define MSTAR_MMU_REG_88	0x88
#define MSTAR_MMU_REG_8C	0x8C
#define MSTAR_MMU_REG_90	0x90
#define MSTAR_MMU_REG_94	0x94
#define MSTAR_MMU_REG_98	0x98
#define MSTAR_MMU_REG_9C	0x9C
#define MSTAR_MMU_REG_A0	0xA0
#define MSTAR_MMU_REG_A4	0xA4
#define MSTAR_MMU_REG_A8	0xA8
#define MSTAR_MMU_REG_100	0x100
#define MSTAR_MMU_REG_10C	0x10C
#define MSTAR_MMU_REG_1FC	0x1FC

int mstar_mmu_init(void);

#endif /* MSTAR_MMU_H_ */
