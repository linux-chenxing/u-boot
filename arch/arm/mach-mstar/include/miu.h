/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef _SOC_MSTAR_MIU_H_
#define _SOC_MSTAR_MIU_H_

enum mstar_dram_type {
	MSTAR_DRAM_SDR,
	MSTAR_DRAM_DDR1,
	MSTAR_DRAM_DDR2,
	MSTAR_DRAM_DDR3,
};

#define MIU_ANA		0x1f202000
#define MIU_EXTRA	0x1f202200
#define MIU_DIG		0x1f202400

#define MIU_ANA				0x1f202000
#define MIU_ANA_00			0x00
#define MIU_ANA_04			0x04
#define MIU_ANA_08			0x08
#define MIU_ANA_10			0x10
#define MIU_ANA_RD_PHASE_TIMING		0x14
#define MIU_ANA_1c			0x1c
#define MIU_ANA_20			0x20
#define MIU_ANA_30			0x30
#define MIU_ANA_34			0x34
#define MIU_ANA_38			0x38
#define MIU_ANA_3C			0x3C
#define MIU_ANA_DDRAT_15_0		0x44
#define MIU_ANA_DDRAT_23_16		0x48
#define MIU_ANA_DDRAT_31_24		0x4c
#define MIU_ANA_50			0x50
#define MIU_ANA_58			0x58
#define MIU_ANA_5c			0x5c
//#define MIU_ANA_60			0x60
//#define MIU_ANA_64			0x64
#define REG_ANA_DDFSET_L		0x60
#define REG_ANA_DDFSET_H		0x64
#define MIU_ANA_68			0x68
#define MIU_ANA_6C			0x6c
#define MIU_ANA_70			0x70
#define MIU_ANA_74			0x74
#define MIU_ANA_78			0x78
#define MIU_ANA_7C			0x7c
#define MIU_ANA_90			0x90
#define MIU_ANA_94			0x94
#define MIU_ANA_98			0x98
#define MIU_ANA_9C			0x9c
#define MIU_ANA_A0			0xa0
#define MIU_ANA_A4			0xa4
#define MIU_ANA_A8			0xa8
#define MIU_ANA_B0			0xb0
#define MIU_ANA_B4			0xb4
#define MIU_ANA_B8			0xb8
#define MIU_ANA_BC			0xbc
#define MIU_ANA_C0			0xc0
#define MIU_ANA_C4			0xc4
#define MIU_ANA_C8			0xc8
#define MIU_ANA_D8			0xd8
#define MIU_ANA_DC			0xdc
#define MIU_ANA_E0			0xe0
#define MIU_ANA_E8			0xe8
#define MIU_ANA_EC			0xec
#define MIU_ANA_F0			0xf0
#define MIU_ANA_F8			0xf8
#define MIU_ANA_114			0x114
#define MIU_ANA_120			0x120
#define MIU_ANA_128			0x128
#define MIU_ANA_130			0x130
#define MIU_ANA_134			0x134
#define MIU_ANA_140			0x140
#define MIU_ANA_144			0x144
#define MIU_ANA_148			0x148
#define MIU_ANA_14C			0x14C
#define MIU_ANA_150			0x150
#define MIU_ANA_154			0x154
#define MIU_ANA_158			0x158
#define MIU_ANA_15C			0x15c
#define MIU_ANA_16C			0x16c
#define MIU_ANA_170			0x170
#define MIU_ANA_174			0x174
#define MIU_ANA_178			0x178
#define MIU_ANA_17C			0x17c
#define MIU_ANA_1A0			0x1a0
#define MIU_ANA_1A4			0x1a4
#define MIU_ANA_1A8			0x1a8
#define MIU_ANA_1AC			0x1ac
#define MIU_ANA_1B0			0x1b0
#define MIU_ANA_1B8			0x1b8
#define MIU_ANA_1C0			0x1c0
#define MIU_ANA_1C4			0x1c4
#define MIU_ANA_1C8			0x1c8
#define MIU_ANA_1CC			0x1cc
#define MIU_ANA_1D0			0x1d0
#define MIU_ANA_1D4			0x1d4
#define MIU_ANA_1D8			0x1d8
#define MIU_ANA_1DC			0x1dc
#define MIU_ANA_1E0			0x1e0
#define MIU_ANA_1E4			0x1e4
#define MIU_ANA_1F0			0x1f0


#define MIU_EXTRA			0x1f202200
#define MIU_EXTRA_GROUP4_CTRL		0x00
#define MIU_EXTRA_GROUP4_REQ_MASK	0x0c
#define MIU_EXTRA_GROUP4_REQ_DEADLINE	0x24
#define MIU_EXTRA_GROUP5_CTRL		0x40
#define MIU_EXTRA_GROUP5_REQ_MASK	0x4c
#define MIU_EXTRA_GROUP5_REQ_DEADLINE	0x6c
#define MIU_EXTRA_C0			0xc0
#define MIU_EXTRA_C4			0xc4
#define MIU_EXTRA_C8			0xc8
#define MIU_EXTRA_CC			0xcc
#define MIU_EXTRA_D0			0xd0
#define MIU_EXTRA_1D0			0x1d0
#define MIU_EXTRA_1D4			0x1d4
#define MIU_EXTRA_1D8			0x1d8
// not sure
#define MIU_EXTRA_GROUP6_CTRL		0x1c0
#define MIU_EXTRA_GROUP6_REQ_MASK	0x1cc

#define MIU_DIG				0x1f202400
#define MIU_DIG_CNTRL0			0x00
#define MIU_DIG_CNTRL0_INIT_MIU		BIT(0)
#define MIU_DIG_CNTRL0_CKE		BIT(1)
#define MIU_DIG_CNTRL0_CS		BIT(2)
#define MIU_DIG_CNTRL0_RSTZ		BIT(3)
#define MIU_DIG_CNTRL0_ODT		BIT(4)
#define MIU_DIG_CNTRL0_INITDONE		BIT(15)

static const struct reg_field miu_odt_field  = REG_FIELD(MIU_DIG_CNTRL0, 4, 4);

#define REG_CONFIG1			0x4

static const struct reg_field miu_memtype_field    = REG_FIELD(REG_CONFIG1, 0, 1);
static const struct reg_field miu_buswidth_field   = REG_FIELD(REG_CONFIG1, 2, 3);
static const struct reg_field miu_banks_field      = REG_FIELD(REG_CONFIG1, 4, 5);
static const struct reg_field miu_cols_field       = REG_FIELD(REG_CONFIG1, 6, 7);
static const struct reg_field miu_data_ratio_field = REG_FIELD(REG_CONFIG1, 8, 9);
static const struct reg_field miu_data_swap_field  = REG_FIELD(REG_CONFIG1, 10, 11);
static const struct reg_field miu_cke_oenz_field   = REG_FIELD(REG_CONFIG1, 12, 12);
static const struct reg_field miu_adr_oenz_field   = REG_FIELD(REG_CONFIG1, 13, 13);
static const struct reg_field miu_dq_oenz_field    = REG_FIELD(REG_CONFIG1, 14, 14);
static const struct reg_field miu_cko_oenz_field   = REG_FIELD(REG_CONFIG1, 15, 15);

static const char* miumemtypetbl[] = {
	"SDR",
	"DDR",
	"DDR2",
	"DDR3",
};

#define REG_CONFIG1_TYPE_SDR		0
#define REG_CONFIG1_TYPE_DDR		BIT(0)
#define REG_CONFIG1_TYPE_DDR2		BIT(1)
#define REG_CONFIG1_TYPE_DDR3		(BIT(1) | BIT(0))

#define REG_CONFIG2			0x8
#define REG_CONFIG2_RD_TIMING		GENMASK(4,0)

static const struct reg_field miu_rdtiming_field = REG_FIELD(REG_CONFIG2, 0, 4);
static const struct reg_field miu_tdqss_field = REG_FIELD(REG_CONFIG2, 5, 6);
static const struct reg_field miu_i64mode_field = REG_FIELD(REG_CONFIG2, 7, 7);
static const struct reg_field miu_mcpen_field = REG_FIELD(REG_CONFIG2, 10, 10);
static const struct reg_field miu_mcptype_field = REG_FIELD(REG_CONFIG2, 11, 11);

#define REG_CONFIG3			0xc
static const struct reg_field miu_trefperiod_field	= REG_FIELD(REG_CONFIG3, 0, 7);
static const struct reg_field miu_tcke_field		= REG_FIELD(REG_CONFIG3, 8, 10);
static const struct reg_field miu_i32mode_field		= REG_FIELD(REG_CONFIG3, 11, 11);
static const struct reg_field miu_ckealwayson_field	= REG_FIELD(REG_CONFIG3, 12, 12);
static const struct reg_field miu_odtalwayson_field	= REG_FIELD(REG_CONFIG3, 13, 13);
static const struct reg_field miu_mobiledram_field	= REG_FIELD(REG_CONFIG3, 14, 14);
static const struct reg_field miu_scrambleen_field	= REG_FIELD(REG_CONFIG3, 15, 15);

#define REG_CONFIG4			0x10
static const struct reg_field miu_trcd_field	= REG_FIELD(REG_CONFIG4, 0, 3);
static const struct reg_field miu_trp_field	= REG_FIELD(REG_CONFIG4, 4, 7);
static const struct reg_field miu_tras_field	= REG_FIELD(REG_CONFIG4, 8, 13);
static const struct reg_field miu_trcdmsb_field	= REG_FIELD(REG_CONFIG4, 14, 14);
static const struct reg_field miu_trpmsb_field	= REG_FIELD(REG_CONFIG4, 15, 15);

#define REG_CONFIG5			0x14
#define REG_CONFIG5_TRRD		GENMASK(3,0)
#define REG_CONFIG5_TRTP		GENMASK(7,4)
#define REG_CONFIG5_TRTP_SHIFT		4
#define REG_CONFIG5_TRC			GENMASK(13, 8)
#define REG_CONFIG5_TRC_SHIFT		8

#define MIU_DIG_TIMING2			0x18

static const struct reg_field miu_twl_field = REG_FIELD(MIU_DIG_TIMING2, 0, 3);
static const struct reg_field miu_twr_field = REG_FIELD(MIU_DIG_TIMING2, 4, 7);
static const struct reg_field miu_twtr_field = REG_FIELD(MIU_DIG_TIMING2, 8, 11);
static const struct reg_field miu_trtw_field = REG_FIELD(MIU_DIG_TIMING2, 12, 15);

#define MIU_DIG_TIMING3			0x1c

static const struct reg_field miu_trfc_field = REG_FIELD(MIU_DIG_TIMING3, 0, 7);
static const struct reg_field miu_txp_field = REG_FIELD(MIU_DIG_TIMING3, 8, 11);
static const struct reg_field miu_tccd_field = REG_FIELD(MIU_DIG_TIMING3, 12, 14);

#define MIU_DIG_MR0			0x20
#define MIU_DIG_MR1			0x24
#define MIU_DIG_MR2			0x28
#define MIU_DIG_MR3			0x2c
#define MIU_DIG_SW_RST			0x3c
#define MIU_DIG_SW_RST_MIU		BIT(0)
#define MIU_DIG_SW_RST_SW_INIT_DONE	BIT(3)
#define MIU_DIG_SW_RST_G0		BIT(4)
#define MIU_DIG_SW_RST_G1		BIT(5)
#define MIU_DIG_SW_RST_G2		BIT(6)
#define MIU_DIG_SW_RST_G3		BIT(7)
#define MIU_DIG_50			0x50
#define MIU_DIG_ADDR_BAL_SEL		0x58
#define MIU_DIG_70			0x70
#define MIU_DIG_GROUP0_CTRL		0x80
#define MIU_DIG_GROUP0_CONFIG0		0x84
#define MIU_DIG_GROUP0_TIMEOUT		0x88
#define MIU_DIG_GROUP0_REQ_MASK		0x8c
#define MIU_DIG_GROUP0_HPMASK		0x90
#define MIU_DIG_GROUP0_REQ_PRIORITY0	0x94
#define MIU_DIG_GROUP0_REQ_PRIORITY1	0x98
#define MIU_DIG_GROUP0_REQ_PRIORITY2	0x9c
#define MIU_DIG_GROUP0_REQ_PRIORITY3	0xa0
#define MIU_DIG_GROUP0_REQ_DEADLINE	0xa4
#define MIU_DIG_GROUP0_REQ_LIMITMASK	0xb8
#define MIU_DIG_GROUP1_CTRL		0xc0
#define MIU_DIG_GROUP1_CONFIG0		0xc4
#define MIU_DIG_GROUP1_TIMEOUT		0xc8
#define MIU_DIG_GROUP1_REQ_MASK		0xcc
#define MIU_DIG_GROUP1_HPMASK		0xd0
#define MIU_DIG_GROUP1_REQ_PRIORITY0	0xd4
#define MIU_DIG_GROUP1_REQ_PRIORITY1	0xd8
#define MIU_DIG_GROUP1_REQ_PRIORITY2	0xdc
#define MIU_DIG_GROUP1_REQ_PRIORITY3	0xe0
#define MIU_DIG_GROUP1_REQ_DEADLINE	0xe4
#define MIU_DIG_GROUP2_CTRL		0x100
#define MIU_DIG_GROUP2_CONFIG0		0x104
#define MIU_DIG_GROUP2_TIMEOUT		0x108
#define MIU_DIG_GROUP2_REQ_MASK		0x10c
#define MIU_DIG_GROUP2_HPMASK		0x110
#define MIU_DIG_GROUP2_REQ_PRIORITY0	0x114
#define MIU_DIG_GROUP2_REQ_PRIORITY1	0x118
#define MIU_DIG_GROUP2_REQ_PRIORITY2	0x11c
#define MIU_DIG_GROUP2_REQ_PRIORITY3	0x120
#define MIU_DIG_GROUP2_REQ_DEADLINE	0x124
#define MIU_DIG_GROUP3_CTRL		0x140
#define MIU_DIG_GROUP3_CONFIG0		0x144
#define MIU_DIG_GROUP3_TIMEOUT		0x148
#define MIU_DIG_GROUP3_REQ_MASK		0x14c
#define MIU_DIG_GROUP3_HPMASK		0x150
#define MIU_DIG_GROUP3_REQ_PRIORITY0	0x154
#define MIU_DIG_GROUP3_REQ_PRIORITY1	0x158
#define MIU_DIG_GROUP3_REQ_PRIORITY2	0x15c
#define MIU_DIG_GROUP3_REQ_PRIORITY3	0x160
#define MIU_DIG_GROUP3_REQ_DEADLINE	0x164
#define MIU_DIG_PROTECT2_START		0x1a4
#define MIU_DIG_MIUSEL0			0x1e0
#define MIU_DIG_PTN_DATA		0x1f8
#define MIU_DIG_R_READ_CRC		0x1fc


#define MIU_M5_DIG_PLLSTATUS		0x98
#define MIU_M5_GROUPS			0x1f202c00
#define MIU_M5_GROUPS_GROUP0_CTRL	0x0
#define MIU_M5_GROUPS_GROUP1_CTRL	0x80
#define MIU_M5_GROUPS_GROUP2_CTRL	0x100
#define MIU_M5_GROUPS_GROUP3_CTRL	0x180
#define MIU_M5_GROUPS_GROUP4_CTRL	0x200
#define MIU_M5_GROUPS_GROUP5_CTRL	0x280
#define MIU_M5_GROUPS_GROUP6_CTRL	0x300

#define MIU_DIG_GROUP_REG_CTRL_OFF	0x00
#define MIU_DIG_GROUP_REG_MASK_OFF	0x0c
#define MIU_DIG_GROUP_REG_DEADLINE_OFF	0x24

#endif
