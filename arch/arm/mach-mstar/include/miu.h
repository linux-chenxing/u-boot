/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef _SOC_MSTAR_MIU_H_
#define _SOC_MSTAR_MIU_H_

enum mstar_dram_type {
	MSTAR_DRAM_SDR,
	MSTAR_DRAM_DDR1,
	MSTAR_DRAM_DDR2,
	MSTAR_DRAM_DDR3,
};

#define MIU_DIG		0x1f202400

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

#define REG_CONFIG6			0x18

static const struct reg_field miu_twl_field = REG_FIELD(REG_CONFIG6, 0, 3);
static const struct reg_field miu_twr_field = REG_FIELD(REG_CONFIG6, 4, 7);
static const struct reg_field miu_twtr_field = REG_FIELD(REG_CONFIG6, 8, 11);
static const struct reg_field miu_trtw_field = REG_FIELD(REG_CONFIG6, 12, 15);

#define REG_CONFIG7			0x1c

static const struct reg_field miu_trfc_field = REG_FIELD(REG_CONFIG7, 0, 7);
static const struct reg_field miu_txp_field = REG_FIELD(REG_CONFIG7, 8, 11);
static const struct reg_field miu_tccd_field = REG_FIELD(REG_CONFIG7, 12, 14);

#define MIU_DIG_MR0			0x20
static const struct reg_field miu_mr0_field = REG_FIELD(MIU_DIG_MR0, 0, 15);
#define MIU_DIG_MR1			0x24
static const struct reg_field miu_mr1_field = REG_FIELD(MIU_DIG_MR1, 0, 15);
#define MIU_DIG_MR2			0x28
static const struct reg_field miu_mr2_field = REG_FIELD(MIU_DIG_MR2, 0, 15);
#define MIU_DIG_MR3			0x2c
static const struct reg_field miu_mr3_field = REG_FIELD(MIU_DIG_MR3, 0, 15);
#define MIU_DIG_MRX			0x30
static const struct reg_field miu_mrx_field = REG_FIELD(MIU_DIG_MRX, 0, 15);

#define MIU_DIG_SW_RST			0x3c
/* For p3 the reset value is 0x8004, the ipl writes 0xc00 3 times */
#define MIU_DIG_SW_RST_VALUE		0xc00
#define MIU_DIG_SW_RST_MIU		BIT(0)
#define MIU_DIG_SW_RST_SW_INIT_DONE	BIT(3)
#define MIU_DIG_SW_RST_G0		BIT(4)
#define MIU_DIG_SW_RST_G1		BIT(5)
#define MIU_DIG_SW_RST_G2		BIT(6)
#define MIU_DIG_SW_RST_G3		BIT(7)
#define MIU_DIG_NO_RQ_CTRL_EN		BIT(15)
static const struct reg_field miu_swrst_syncoutthreshold = REG_FIELD(MIU_DIG_SW_RST, 8, 12);



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

#define MIU_DIG_PROTECT0_ID_0_1		0x184
#define MIU_DIG_PROTECT0_ID_2_3		0x188
#define MIU_DIG_PROTECT0_START		0x18c
#define MIU_DIG_PROTECT0_END		0x190
#define MIU_DIG_PROTECT1_ID		0x194
#define MIU_DIG_PROTECT1_START		0x198
#define MIU_DIG_PROTECT1_END		0x19c
#define MIU_DIG_PROTECT2_ID		0x1a0

#define MIU_DIG_PROTECT2_START		0x1a4
static const struct reg_field miu_protect2_start_field = REG_FIELD(MIU_DIG_PROTECT2_START, 0, 15);

#define MIU_DIG_PROTECT2_END		0x1a8
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
