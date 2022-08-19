/*
 *
 */

#include <chenxingv7.h>
#include "mmu.h"

static void mstar_mmu_dump(void) {
	printk("mmu dump\n");
	for (int i = 0; i < 0x200; i += 4){
		unsigned int value = readw(MSTAR_MMU + i);
		printf("mmu dump: 0x%08x - 0x%04x\n", MSTAR_MMU + i, value);

	}
}

int mstar_mmu_init(void) {
	mstar_mmu_dump();

	/* This seems to be the standard setup for the MMU */
	mstar_writew(0x3, MSTAR_MMU + MSTAR_MMU_REG_44);
	mstar_writew(0x0, MSTAR_MMU + MSTAR_MMU_REG_7C);
	mstar_writew(0x909, MSTAR_MMU + MSTAR_MMU_REG_80);
	mstar_writew(0x71e, MSTAR_MMU + MSTAR_MMU_REG_84);
	mstar_writew(0x2707, MSTAR_MMU + MSTAR_MMU_REG_88);
	mstar_writew(0x0908, MSTAR_MMU + MSTAR_MMU_REG_8C);
	mstar_writew(0x0905, MSTAR_MMU + MSTAR_MMU_REG_90);
	mstar_writew(0x0304, MSTAR_MMU + MSTAR_MMU_REG_94);
	mstar_writew(0x0528, MSTAR_MMU + MSTAR_MMU_REG_98);
	mstar_writew(0x0046, MSTAR_MMU + MSTAR_MMU_REG_9C);
	mstar_writew(0xe000, MSTAR_MMU + MSTAR_MMU_REG_A0);
	mstar_writew(0x0000, MSTAR_MMU + MSTAR_MMU_REG_A4);
	mstar_writew(0x0900, MSTAR_MMU + MSTAR_MMU_REG_A8);
	mstar_writew(0x0000, MSTAR_MMU + MSTAR_MMU_REG_100);
	mstar_writew(0x0000, MSTAR_MMU + MSTAR_MMU_REG_10C);
	mstar_writew(0x0000, MSTAR_MMU + MSTAR_MMU_REG_1FC);

	return 0;
}
