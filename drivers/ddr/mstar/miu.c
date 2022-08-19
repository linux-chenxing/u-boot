#include <common.h>
#include <dm.h>
#include <ram.h>
#include <regmap.h>
#include <clk.h>
#include <asm/io.h>
#include <chenxingv7.h>

#include "arb.h"
#include "clk.h"
#include "debug.h"
#include "miu.h"
#include "mmu.h"

#define NUM_GROUPS 8

struct ddr_config {
	uint32_t size;

	enum mstar_dram_type type;
	unsigned banks;
	unsigned cols;

	void *group_registers[NUM_GROUPS];
	int group_init_mask[NUM_GROUPS];

	//int dig_config2;
	unsigned int rdtiming;
	unsigned int tdqss;
	unsigned int twr;
	unsigned int twl;
	unsigned int twtr;
	unsigned int trtw;
	unsigned int trfc;
	unsigned int txp;
	unsigned int tccd;

	uint16_t mr0, mr1, mr2, mr3;
	uint16_t rd_phase_timing;
	uint16_t ptn_mode;
	uint16_t ana_5c, ana_d8, ana_dc, ana_ec;
	uint16_t ana_120, ana_128;
	uint16_t ana_148, ana_14c;
	uint16_t ana_150, ana_154, ana_158, ana_15c;
	uint16_t ana_170, ana_174, ana_178, ana_17c;
	uint16_t ana_1f0;
};

struct mstar_miu_priv {
	struct regmap	*ana, *dig, *arb;
	struct clk	miupll;

	/* ctrl */
	struct regmap_field *odt;
	/* config 1 */
	struct regmap_field *memtype, *buswidth, *banks, *cols,
		*data_ratio, *data_swap, *cke_oenz, *adr_oenz, *dq_oenz,
		*cko_oenz;
	/* config 2 */
	struct regmap_field *rdtiming, *tdqss, *i64mode, *mcpen, *mcptype;
	/* config 3 */
	struct regmap_field *trefperiod, *tcke, *i32mode, *ckealwayson,
		*odtalwayson, *mobiledram, *scrambleen;
	/* config 4 */
	struct regmap_field *trp, *trcd, *tras, *trcdmsb, *trpmsb;

	struct regmap_field *twr, *twl, *twtr, *trtw;
	struct regmap_field *trfc, *txp, *tccd;

	/* mode registers */
	struct regmap_field *mr0, *mr1, *mr2, *mr3, *mrx;

	/* protect2 registers */
	struct regmap_field *protect2_start;
};

/* -- confirmed in SSD210 ipl code -- */
static inline void miu_dig_rst(struct regmap *dig)
{
	int i;

	/* IPL writes the default value multiple times */
	for (i = 0; i < 3; i++)
		mstar_regmap_write(dig, MIU_DIG_SW_RST,
				MIU_DIG_SW_RST_VALUE);

	/* Then sets reset */
	mstar_regmap_write(dig, MIU_DIG_SW_RST,
			MIU_DIG_SW_RST_MIU |
			MIU_DIG_SW_RST_VALUE);
}

static void mstar_ddr_dig_rst_release(struct regmap *dig)
{
	/*
	 * Not sure if this is needed by the IPL sets the NO_RQ_CTRL_EN bit
	 * and then releases reset.
	 */
	mstar_regmap_write(dig,  MIU_DIG_SW_RST,
			MIU_DIG_NO_RQ_CTRL_EN |
			MIU_DIG_SW_RST_MIU |
			MIU_DIG_SW_RST_VALUE);

	mstar_regmap_write(dig,  MIU_DIG_SW_RST,
			MIU_DIG_NO_RQ_CTRL_EN |
			MIU_DIG_SW_RST_VALUE);
}

static inline void mstar_ddr_setrqvalues(const struct ddr_config *config, const int *values, int reg)
{
	int i;

	for (i = 0; i < NUM_GROUPS; i++) {
		if (values[i] >= 0 && config->group_registers[i] != NULL)
			mstar_writew(values[i],
				(uint32_t)config->group_registers[i] + reg);
	}
}

static void mstar_ddr_setrqctrl(const struct ddr_config *config, const int *values)
{
	mstar_ddr_setrqvalues(config, values, MIU_DIG_GROUP_REG_CTRL_OFF);
}

static void mstar_ddr_setrequestmasks(const struct ddr_config *config, const int *values)
{
	mstar_ddr_setrqvalues(config, values, MIU_DIG_GROUP_REG_MASK_OFF);
}

static void mstar_ddr_setrqdeadlines(const struct ddr_config *config, const int *values)
{
	mstar_ddr_setrqvalues(config, values, MIU_DIG_GROUP_REG_DEADLINE_OFF);
}

/*
 * Confirmed to be in IPL for:
 * - Pioneer 3
 */
static void mstar_ddr_setinitrequestmasks(const struct ddr_config *config)
{
	mstar_ddr_setrequestmasks(config, config->group_init_mask);
}

/*
 * Confirmed to be in IPL for:
 * - Pioneer3
 */
static inline void miu_ana_reset(const struct mstar_miu_priv *miu)
{
	/* drive cal software mode */
	mstar_regmap_write(miu->ana, MIU_ANA_F0, 1);
	mstar_delay(1000);
	mstar_regmap_write(miu->ana, MIU_ANA_DDRAT_23_16, 0x1000);
	mstar_delay(1000);
	mstar_regmap_write(miu->ana, MIU_ANA_DDRAT_23_16, 0);
	mstar_delay(1000);
}

/* -- confirmed -- */

static bool mstar_ddr_isconfigured(const struct mstar_miu_priv *miu, const struct ddr_config *config)
{
	unsigned int memtype;

	regmap_field_read(miu->memtype, &memtype);

	if (memtype == MSTAR_DRAM_DDR2)
		return true;

	return false;
}

static void miu_memory_config(const struct mstar_miu_priv *miu,
		const struct ddr_config *config)
{
	/* Config 1 */
	mstar_regmap_field_write(miu->memtype, MSTAR_DRAM_DDR2);
	mstar_regmap_field_write(miu->buswidth, 0);
	mstar_regmap_field_write(miu->banks, 1);
	mstar_regmap_field_write(miu->cols, 2);
	mstar_regmap_field_write(miu->data_ratio, 2);
	mstar_regmap_field_write(miu->data_swap, 0);
	mstar_regmap_field_write(miu->cke_oenz, 0);
	mstar_regmap_field_write(miu->adr_oenz, 0);
	mstar_regmap_field_write(miu->dq_oenz, 0);
	mstar_regmap_field_write(miu->cko_oenz, 0);

	/* Config 2 */
	mstar_regmap_field_write(miu->rdtiming, config->rdtiming);
	mstar_regmap_field_write(miu->tdqss, config->tdqss);
	mstar_regmap_field_write(miu->i64mode, 0);
	mstar_regmap_field_write(miu->mcpen, 0);
	mstar_regmap_field_write(miu->mcptype, 0);

	/* Config 3 */
	mstar_regmap_write(miu->dig, REG_CONFIG3, 0x1b50);

	/* Config 4 */
	mstar_regmap_write(miu->dig, REG_CONFIG4, 0x1e99);

	/* Config 5 */
	mstar_regmap_write(miu->dig, REG_CONFIG5, 0x2777);

	/* Config 6 */
	mstar_regmap_field_write(miu->twl, config->twl);
	mstar_regmap_field_write(miu->twr, config->twr);
	mstar_regmap_field_write(miu->twtr, config->twtr);
	mstar_regmap_field_write(miu->trtw, config->trtw);

	/* Config 7 */
	mstar_regmap_field_write(miu->trfc, config->trfc);
	mstar_regmap_field_write(miu->txp, config->txp);
	mstar_regmap_field_write(miu->tccd, config->tccd);

	/* mode registers */
	mstar_regmap_field_write(miu->mr0, config->mr0);
	mstar_regmap_field_write(miu->mr1, config->mr1);
	mstar_regmap_field_write(miu->mr2, config->mr2);
	mstar_regmap_field_write(miu->mr3, config->mr3);

	/* not sure yet, in p3 ipl */
	mstar_regmap_write(miu->dig, MIU_DIG_50, 0x0070);

	/* protect2 */
	mstar_regmap_field_write(miu->protect2_start, 0x6000);

	// dgp, probably need to be moved.
	//mstar_regmap_write(miu->dig, MIU_DIG_ADDR_BAL_SEL, 0x8021);
	//mstar_regmap_write(miu->dig, MIU_DIG_70, 0x0);
	//


	// timing configuration
	//mstar_regmap_write(miu->dig, MIU_DIG_TIMING2, 0x9598);
	//mstar_regmap_write(miu->dig, MIU_DIG_TIMING3, 0x4046);

	// 2450, only on m5?
	// also in p3 but maybe not here?
	//mstar_regmap_writew(miu->ana, MIU_ANA_50, 0x0020);
}

static int mstar_ddr_doinitialcycle(const struct mstar_miu_priv *miu,
		const struct ddr_config *config)
{
	int loops;
	uint tmp;

	// clear
	tmp = 0;
	mstar_regmap_write(miu->dig, MIU_DIG_CNTRL0, tmp);
	mstar_delay(100);

	// deassert rst
	tmp |= MIU_DIG_CNTRL0_RSTZ;
	mstar_regmap_write(miu->dig, MIU_DIG_CNTRL0, tmp);
	mstar_delay(100);

	// assert cs
	tmp |= MIU_DIG_CNTRL0_CS;
	mstar_regmap_write(miu->dig, MIU_DIG_CNTRL0, tmp);
	mstar_delay(100);

	// enable the clock
	tmp |= MIU_DIG_CNTRL0_CKE;
	mstar_regmap_write(miu->dig, MIU_DIG_CNTRL0, 0xe);
	mstar_delay(100);

	// enable odt and trigger init cycle

	// p3 doesn't use ODT?
	tmp |= MIU_DIG_CNTRL0_INIT_MIU; //| MIU_DIG_CNTRL0_ODT;

	mstar_regmap_write(miu->dig, MIU_DIG_CNTRL0, tmp);
	printf("waiting for init to complete.. ");
	for(loops = 0; loops < 10; loops++){
		regmap_read(miu->dig, MIU_DIG_CNTRL0, &tmp);
		if(tmp & MIU_DIG_CNTRL0_INITDONE){
			printf("done\n");
			return 0;
		}
		mdelay(100);
	}
	printf("failed\n");
	return -1;
}

static inline void mstar_ddr_setclkfreq(const struct mstar_miu_priv *miu,
		const struct ddr_config *config)
{
	uint16_t pllstatus = readw(MIU_DIG + MIU_M5_DIG_PLLSTATUS);

	/* ddr clock freq */
	mstar_regmap_write(miu->ana, MIU_ANA_6C, 0x0400);
	mstar_regmap_write(miu->ana, MIU_ANA_68, 0x2004);

	/* not setting this causes the initial cycle to not finish */
	mstar_regmap_write(miu->ana, MIU_ANA_114, 0x1);

	// clock gen freq set
	mstar_writew(0x8000, MIU_ANA + REG_ANA_DDFSET_L); // p3 ipl only does this for bond == 0x13?
	mstar_writew(0x29, MIU_ANA + REG_ANA_DDFSET_H);
	mstar_delay(100);

#if 0 //not in p3 ipl
	mstar_writew(0x1, MIU_ANA + MIU_ANA_114);

	// only on m5?
	mstar_writew(0x0010, MIU_ANA + MIU_ANA_BC);
#endif

	mstar_writew(0x4, MIU_ANA + MIU_ANA_DDRAT_15_0);

	mstar_delay(100);

	mstar_writew(0x114, MIU_ANA + MIU_ANA_58);

	printf("pll status before: %04x", pllstatus);
	pllstatus = readw(MIU_DIG + MIU_M5_DIG_PLLSTATUS);
	printf(" after: %04x\n", pllstatus);
}

static void mstar_ddr_analogconfig(const struct ddr_config *config)
{
#if 0 //disable while debugging p3
	mstar_writew(0x800, MIU_ANA + MIU_ANA_E0);

	// m5 only
	mstar_writew(0x0404, 0x1f202b40);
	mstar_writew(0x0404, 0x1f202b44);
	mstar_writew(0x0404, 0x1f202b48);
	mstar_writew(0x0404, 0x1f202b4c);
	mstar_writew(0x0a0a, 0x1f202b50);
	mstar_writew(0x0404, 0x1f202b54);
	mstar_writew(0x0404, 0x1f202b58);
	mstar_writew(0x0404, 0x1f202b5c);
	mstar_writew(0x0404, 0x1f202b60);
	mstar_writew(0x0a0a, 0x1f202b64);
	// reserved m5 0707
	mstar_writew(config->ana_d8, MIU_ANA + MIU_ANA_D8);
	// phase delay select, m5 0707
	mstar_writew(config->ana_dc, MIU_ANA + MIU_ANA_DC);

	// rec trig
	mstar_writew(0x202, MIU_ANA + MIU_ANA_E8);
	mstar_writew(config->ana_ec, MIU_ANA + MIU_ANA_EC);
	mstar_writew(0x20, MIU_ANA + MIU_ANA_38);

	// ptn mode m5 8020
	mstar_writew(config->ptn_mode, MIU_ANA + MIU_ANA_34);

	// rx en
	mstar_writew(0x3f, MIU_ANA + MIU_ANA_10);
#endif
}

static inline void miu_ana_powerup(const struct mstar_miu_priv *miu,
		const struct ddr_config *config)
{
	/* Confirmed in P3 ipl */

	mstar_writew(0x2010, MIU_ANA + MIU_ANA_00);
	mstar_writew(0x0000, MIU_ANA + MIU_ANA_00);
	mstar_writew(0x0000, MIU_ANA + MIU_ANA_30);
	mstar_writew(0x0000, MIU_ANA + MIU_ANA_F8);
	mstar_writew(0x4000, MIU_ANA + MIU_ANA_A8);
	mstar_writew(0x0005, MIU_ANA + MIU_ANA_3C);
	mstar_writew(0x000f, MIU_ANA + MIU_ANA_3C);
	mstar_writew(0x0005, MIU_ANA + MIU_ANA_3C);
	// power up ana
	mstar_writew(0x1, MIU_ANA + MIU_ANA_00);
}

static void mstar_ddr_unmask_bist(struct ddr_config *config)
{
	const int values[] = {
		0x7fff,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		0xfffa,
	};
	mstar_ddr_setrequestmasks(config, values);
}

static void mstar_ddr_setdone(struct ddr_config *config)
{
	uint16_t temp;
	/* if this is not cleared any access to the DDR locks the CPU */
	temp = readw(MIU_DIG + MIU_DIG_SW_RST);
	temp |= 0xc00;
	temp |= MIU_DIG_SW_RST_SW_INIT_DONE;
	mstar_writew(temp, MIU_DIG + MIU_DIG_SW_RST);
}

static void mstar_ddr_fillconfig_infinity_pioneer3(struct ddr_config *config)
{
	config->group_registers[0] = (void*) MIU_DIG + MIU_DIG_GROUP0_CTRL;
	config->group_registers[1] = (void*) MIU_DIG + MIU_DIG_GROUP1_CTRL;
	config->group_registers[2] = (void*) MIU_DIG + MIU_DIG_GROUP2_CTRL;
	config->group_registers[3] = (void*) MIU_DIG + MIU_DIG_GROUP3_CTRL;
	config->group_registers[4] = (void*) MIU_ARB + MIU_ARB_GROUP4_CTRL;
	config->group_registers[5] = (void*) MIU_ARB + MIU_ARB_GROUP5_CTRL;
	config->group_registers[6] = (void*) MIU_ARB + MIU_ARB_GROUP6_CTRL;


	//config->group7 = NULL;

	config->group_init_mask[0] = 0xfffe;
	config->group_init_mask[1] = 0xffff;
	config->group_init_mask[2] = 0xffff;
	config->group_init_mask[3] = 0xffff;
	config->group_init_mask[4] = 0xffff;
	config->group_init_mask[5] = 0xffff;
	config->group_init_mask[6] = 0xfffe;
}

static void mstar_ddr_fillconfig_ssd210(struct ddr_config *config)
{
	config->size = 0x4000000; // 64MB
	config->type = MSTAR_DRAM_DDR2;

	mstar_ddr_fillconfig_infinity_pioneer3(config);

	// new
	config->rdtiming = 0x11;
	config->tdqss = 0x2;
	config->twl = 0x8;
	config->twr = 0xa;
	config->twtr = 0x5;
	config->trtw = 0x9;
	config->trfc = 0x4c;
	config->txp = 0;
	config->tccd = 0x4;
	//

	config->mr0 = 0x3;
	config->mr1 = 0x4004;
	config->mr2 = 0x8000;
	config->mr3 = 0xc000;

	config->rd_phase_timing = 0x0;

	config->ptn_mode = 0x8020;

	config->ana_5c = 0x2222;

	config->ana_d8 = 0x707;
	config->ana_dc = 0x707;

	config->ana_ec = 0x707;

	config->ana_120 = 0xf0f3;
	config->ana_128 = 0x2829;

	config->ana_148 = 0;
	config->ana_14c = 0;

	config->ana_150 = 0;
	config->ana_154 = 0;
	config->ana_158 = 0;
	config->ana_15c = 0x202;

	config->ana_170 = 0;
	config->ana_174 = 0;
	config->ana_178 = 0;
	config->ana_17c = 0;
}

static int mstar_ddr_getconfig(int chiptype, struct ddr_config *config)
{
	memset(config, 0, sizeof(*config));

	printf("trying to work out DDR config..\n");

	switch(chiptype){
	case CHIPTYPE_MSC313E:
		mstar_ddr_fillconfig_infinity_pioneer3(config);

		//config->dig_config2 = 0x1b28;
		break;
#ifdef CONFIG_MSTAR_MERCURY5
	case CHIPTYPE_SSC8336:
	case CHIPTYPE_SSC8336N:
		type = readw(GPIO + GPIO_18);
		printf("mystery gpio register is %02x\n", type);
		type &= GPIO_18_D9_DDRMASK;

		switch (chiptype) {
		case CHIPTYPE_SSC8336:
			switch (type) {
			case 0:
				config->size = 0x2000000; // 32MB
				break;
			case 1:
				config->size = 0x4000000; // 64MB
				config->type = MSTAR_DRAM_DDR2;
				break;
			case 2:
				config->size = 0x8000000; // 128MB
				break;
			case 4:
				config->size = 0x10000000; // 256MB
				break;
			default:
				printf("Don't know how to workout the DDR size for %02x\n",type);
				break;
			}
			break;
		case CHIPTYPE_SSC8336N:
			switch (type) {
			case 0xf:
				config->size = 0x4000000; // 64MB
				config->type = MSTAR_DRAM_DDR2;
				break;
			}
			break;
		}



		config->group0 = (void*) MIU_M5_GROUPS + MIU_M5_GROUPS_GROUP0_CTRL;
		config->group1 = (void*) MIU_M5_GROUPS + MIU_M5_GROUPS_GROUP1_CTRL;
		config->group2 = (void*) MIU_M5_GROUPS + MIU_M5_GROUPS_GROUP2_CTRL;
		config->group3 = (void*) MIU_M5_GROUPS + MIU_M5_GROUPS_GROUP3_CTRL;
		config->group4 = (void*) MIU_M5_GROUPS + MIU_M5_GROUPS_GROUP4_CTRL;
		config->group5 = (void*) MIU_M5_GROUPS + MIU_M5_GROUPS_GROUP5_CTRL;
		config->group6 = (void*) MIU_M5_GROUPS + MIU_M5_GROUPS_GROUP6_CTRL;
		config->group7 = (void*) MIU_ARB + MIU_ARB_GROUP6_CTRL;

		config->group0_init_mask = 0xfffe;
		config->group1_init_mask = 0xffff;
		config->group2_init_mask = 0xffff;
		config->group3_init_mask = 0xffff;
		config->group4_init_mask = 0xffff;
		config->group5_init_mask = 0xffff;
		config->group6_init_mask = 0xffff;
		config->group7_init_mask = 0xfffe;

		config->dig_config2 = 0x1828;

		config->mr0 = 0x3;
		config->mr1 = 0x4004;
		config->mr2 = 0x8000;
		config->mr3 = 0xc000;

		config->rd_phase_timing = 0x0;

		config->ptn_mode = 0x8020;

		config->ana_5c = 0x2222;

		config->ana_d8 = 0x707;
		config->ana_dc = 0x707;

		config->ana_ec = 0x707;

		config->ana_120 = 0xf0f3;
		config->ana_128 = 0x2829;

		config->ana_148 = 0;
		config->ana_14c = 0;

		config->ana_150 = 0;
		config->ana_154 = 0;
		config->ana_158 = 0;
		config->ana_15c = 0x202;

		config->ana_170 = 0;
		config->ana_174 = 0;
		config->ana_178 = 0;
		config->ana_17c = 0;
		break;
#endif
#ifdef CONFIG_MSTAR_PIONEER3
		case CHIPTYPE_SSD210:
			mstar_ddr_fillconfig_ssd210(config);
		break;
#endif
	default:
		printf("Don't know how to find DRAM config for chiptype %i\n", chiptype);
		return -EINVAL;
	}

	printf("Detected DRAM: %08x bytes\n", config->size);

	return 0;
}

static inline void miu_configure_rqs(const struct mstar_miu_priv *miu, const struct ddr_config *config)
{
	const int rqdeadlines[] = {
		0xffff,
		0xffff,
		0xffff,
		0x0,
		0xffff,
		0xffff,
		-1,
	};

	const int rqsettings[] = {
		0x8015,
		0x8015,
		0x8015,
		0x8015,
		0x8015,
		0x8015,
		-1,
	};

	mstar_ddr_setrqdeadlines(config, rqdeadlines);
	mstar_ddr_setrqctrl(config, rqsettings);
}

static inline void miu_unmask_rqs(const struct mstar_miu_priv *priv, const struct ddr_config *config)
{
	const int workingrequestmasks[] = {
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
	};
	mstar_ddr_setrequestmasks(config, workingrequestmasks);
}

static void miu_ana_mystery(const struct mstar_miu_priv *priv, const struct ddr_config *config)
{
	// clock wave form
	mstar_writew(0xaaaa, MIU_ANA + MIU_ANA_04);
	mstar_writew(0x0, MIU_ANA + MIU_ANA_08);
	// more timing
	mstar_writew(0x85, MIU_ANA + MIU_ANA_1c);
	// reserved , m5 is 2222
	mstar_writew(config->ana_5c, MIU_ANA + MIU_ANA_5c);

	mstar_writew(config->ana_128, MIU_ANA + MIU_ANA_128);

	mstar_writew(0x0304, 0x1f202b00);
	mstar_writew(0x0200, 0x1f202b04);
	mstar_writew(0x0404, 0x1f202b08);
	mstar_writew(0x0304, 0x1f202b0c);
	mstar_writew(0x0201, 0x1f202b10);
	mstar_writew(0x0101, 0x1f202b14);
	mstar_writew(0x0101, 0x1f202b18);
	mstar_writew(0x0303, 0x1f202b1c);

	mstar_writew(0x0, MIU_ANA + MIU_ANA_16C);

	mstar_writew(0x0002, MIU_ANA + MIU_ANA_1B8);
	mstar_writew(0x0011, MIU_ANA + MIU_ANA_1C0);
	mstar_writew(0x0000, MIU_ANA + MIU_ANA_1C4);
	mstar_writew(0x0010, MIU_ANA + MIU_ANA_1C8);
	mstar_writew(0x1111, MIU_ANA + MIU_ANA_1CC);
	mstar_writew(0x1111, MIU_ANA + MIU_ANA_1D0);
	mstar_writew(0x1111, MIU_ANA + MIU_ANA_1D4);
	mstar_writew(0x1111, MIU_ANA + MIU_ANA_1D8);
	mstar_writew(0x1111, MIU_ANA + MIU_ANA_1DC);
	mstar_writew(0x3333, MIU_ANA + MIU_ANA_1E0);
	mstar_writew(0x0033, MIU_ANA + MIU_ANA_1E4);


	mstar_writew(0x0000, 0x1f202a00);
	mstar_writew(0x0000, 0x1f202a08);
	mstar_writew(0x0000, 0x1f202a10);
	mstar_writew(0x0000, 0x1f202a14);
	mstar_writew(0x0000, 0x1f202a18);
	mstar_writew(0x0200, 0x1f202a20);
	mstar_writew(0x0000, 0x1f202a24);
	mstar_writew(0x0505, 0x1f202a28);
	mstar_writew(0x0000, 0x1f202a3c);
	mstar_writew(0x0505, 0x1f202a40);
	mstar_writew(0x0505, 0x1f202a44);
	mstar_writew(0x0505, 0x1f202a48);
	mstar_writew(0x0505, 0x1f202a4c);
	mstar_writew(0x0505, 0x1f202a50);
	mstar_writew(0x0505, 0x1f202a54);
	mstar_writew(0x0505, 0x1f202a58);
	mstar_writew(0x0505, 0x1f202a5c);
	mstar_writew(0x0202, 0x1f202ac0);
	mstar_writew(0x0000, 0x1f202ac4);
	mstar_writew(0x0808, 0x1f202ac8);
	mstar_writew(0x0808, 0x1f202acc);
}

static void miu_configure_dram(const struct mstar_miu_priv *miu)
{
	int chiptype =  mstar_chiptype();
	struct regmap *dig = miu->dig;
	struct ddr_config config;

	if (mstar_ddr_getconfig(chiptype, &config))
		goto out;

	if (mstar_ddr_isconfigured(miu, &config)) {
		printf("MIU seems to be configured already, leaving alone\n");
		return;
	}

	/* Put the digital part into reset */
	miu_dig_rst(dig);

	/* Configure the initial request masks so everything is blocked */
	mstar_ddr_setinitrequestmasks(&config);

	/* Reset the analog section */
	miu_ana_reset(miu);

	/* Fire up the ddrpll */
	mstar_ddr_setclkfreq(miu, &config);

	/* Configure the digital section for the connected memory */
	miu_memory_config(miu, &config);

	mstar_mmu_init();

	mstar_arb_mystery(miu->arb);

	miu_ana_mystery(miu, &config);

	miu_configure_rqs(miu, &config);

	mstar_ddr_analogconfig(&config);
	mstar_ddr_dig_rst_release(dig);

	miu_ana_powerup(miu, &config);
	if (mstar_ddr_doinitialcycle(miu, &config))
		goto out;

	mstar_ddr_setdone(&config);
	mstar_ddr_unmask_bist(&config);

	mstar_writew(0x8000, 0x1f2025fc);
	mstar_writew(0x200, 0x1f2025ac);
	mstar_writew(0x000, 0x1f2025e0);

	printf("-----\n");

	mstar_delay(1000);


	miu_unmask_rqs(miu, &config);

	//mstar_dump_reg_block("miu_dig", MIU_DIG);
	//mstar_dump_reg_block("miu_dig", MIU_DIG + 0x200);

	mstar_writew(0x8, CLKGEN + CLKGEN_MIU);
	mstar_writew(0x18, CLKGEN + CLKGEN_MIU);
	mstar_writew(0x4, CLKGEN + CLKGEN_MIU_BOOT);

	// This will lock up until the cpu clock is setup.
	//mstar_ddr_test();

out:
	return;
}


#ifndef CONFIG_MSTAR_IPL

#ifndef CONFIG_SPL_RAM
#error "SPL_RAM must be selected"
#endif

static ulong mstar_miu_ddrpll_recalc_rate(struct mstar_miu_priv *miu, unsigned long parent_rate)
{
	// no idea if this calculation is correct
	unsigned int tmp;
	u64 freq, base;
	u32 ddfset;

	base = (((u64)parent_rate * 4 * 4) << 19);
	regmap_read(miu->ana, REG_ANA_DDFSET_H, &tmp);
	ddfset = (tmp & 0xff) << 16;
	regmap_read(miu->ana, REG_ANA_DDFSET_L, &tmp);
	ddfset |= tmp;

	freq = base / ddfset;

	return freq;
}

static int mstar_miu_probe(struct udevice *dev)
{
	struct mstar_miu_priv *priv = dev_get_priv(dev);
	ulong miupllrate, ddrpllrate;
	unsigned int memtype, buswidth, banks, cols;
	unsigned int trcd, trp, tras, trcdmsb, trpmsb;
	int ret;

	ret = regmap_init_mem_index(dev_ofnode(dev), &priv->ana, 0);
	if (ret)
		goto out;

	ret = regmap_init_mem_index(dev_ofnode(dev), &priv->dig, 1);
	if (ret)
		goto out;

	ret = regmap_init_mem_index(dev_ofnode(dev), &priv->arb, 2);
	if (ret)
		goto out;

	ret = clk_get_by_name(dev, "miupll", &priv->miupll);
	if (ret)
		goto out;

	/* Ctrl */
	priv->odt = devm_regmap_field_alloc(dev, priv->dig, miu_odt_field);

	/* Config 1 */
	priv->memtype = devm_regmap_field_alloc(dev, priv->dig, miu_memtype_field);
	priv->buswidth = devm_regmap_field_alloc(dev, priv->dig, miu_buswidth_field);
	priv->banks = devm_regmap_field_alloc(dev, priv->dig, miu_banks_field);
	priv->cols = devm_regmap_field_alloc(dev, priv->dig, miu_cols_field);
	priv->data_ratio = devm_regmap_field_alloc(dev, priv->dig, miu_data_ratio_field);
	priv->data_swap = devm_regmap_field_alloc(dev, priv->dig, miu_data_swap_field);
	priv->cke_oenz = devm_regmap_field_alloc(dev, priv->dig, miu_cke_oenz_field);
	priv->adr_oenz = devm_regmap_field_alloc(dev, priv->dig, miu_adr_oenz_field);
	priv->dq_oenz = devm_regmap_field_alloc(dev, priv->dig, miu_dq_oenz_field);
	priv->cko_oenz = devm_regmap_field_alloc(dev, priv->dig, miu_cko_oenz_field);

	/* Config 2 */
	priv->rdtiming = devm_regmap_field_alloc(dev, priv->dig, miu_rdtiming_field);
	priv->tdqss = devm_regmap_field_alloc(dev, priv->dig, miu_tdqss_field);
	priv->i64mode = devm_regmap_field_alloc(dev, priv->dig, miu_i64mode_field);
	priv->mcpen = devm_regmap_field_alloc(dev, priv->dig, miu_mcpen_field);
	priv->mcptype = devm_regmap_field_alloc(dev, priv->dig, miu_mcptype_field);

	/* Config 3 */
	priv->trefperiod = devm_regmap_field_alloc(dev, priv->dig, miu_trefperiod_field);
	priv->tcke = devm_regmap_field_alloc(dev, priv->dig, miu_tcke_field);
	priv->i32mode = devm_regmap_field_alloc(dev, priv->dig, miu_i32mode_field);
	priv->ckealwayson = devm_regmap_field_alloc(dev, priv->dig, miu_ckealwayson_field);
	priv->odtalwayson = devm_regmap_field_alloc(dev, priv->dig, miu_odtalwayson_field);
	priv->mobiledram = devm_regmap_field_alloc(dev, priv->dig, miu_mobiledram_field);
	priv->scrambleen = devm_regmap_field_alloc(dev, priv->dig, miu_scrambleen_field);

	/* Config 4 */
	priv->trcd = devm_regmap_field_alloc(dev, priv->dig, miu_trcd_field);
	priv->trp = devm_regmap_field_alloc(dev, priv->dig, miu_trp_field);
	priv->tras = devm_regmap_field_alloc(dev, priv->dig, miu_tras_field);
	priv->trcdmsb = devm_regmap_field_alloc(dev, priv->dig, miu_trcdmsb_field);
	priv->trpmsb = devm_regmap_field_alloc(dev, priv->dig, miu_trpmsb_field);

	priv->twl = devm_regmap_field_alloc(dev, priv->dig, miu_twl_field);
	priv->twr = devm_regmap_field_alloc(dev, priv->dig, miu_twr_field);
	priv->twtr = devm_regmap_field_alloc(dev, priv->dig, miu_twtr_field);
	priv->trtw = devm_regmap_field_alloc(dev, priv->dig, miu_trtw_field);

	priv->trfc = devm_regmap_field_alloc(dev, priv->dig, miu_trfc_field);
	priv->txp = devm_regmap_field_alloc(dev, priv->dig, miu_txp_field);
	priv->tccd = devm_regmap_field_alloc(dev, priv->dig, miu_tccd_field);

	/* Mode registers */
	priv->mr0 = devm_regmap_field_alloc(dev, priv->dig, miu_mr0_field);
	priv->mr1 = devm_regmap_field_alloc(dev, priv->dig, miu_mr1_field);
	priv->mr2 = devm_regmap_field_alloc(dev, priv->dig, miu_mr2_field);
	priv->mr3 = devm_regmap_field_alloc(dev, priv->dig, miu_mr3_field);
	priv->mrx = devm_regmap_field_alloc(dev, priv->dig, miu_mrx_field);

	/* protect2 registers */
	priv->protect2_start = devm_regmap_field_alloc(dev, priv->dig, miu_protect2_start_field);

	clk_enable(&priv->miupll);

	miu_configure_dram(priv);

	miupllrate = clk_get_rate(&priv->miupll);
	ddrpllrate = mstar_miu_ddrpll_recalc_rate(priv, miupllrate);
	printk("MIUPLL running at %uMHz\n", (unsigned int)(miupllrate/1000000));
	printk("DDRPLL running at %uMHz\n", (unsigned int)(ddrpllrate/1000000));

	regmap_field_read(priv->memtype, &memtype);
	regmap_field_read(priv->buswidth, &buswidth);
	regmap_field_read(priv->banks, &banks);
	regmap_field_read(priv->cols, &cols);

	buswidth += 1;
	buswidth *= 16;
	banks = 2 << banks;
	cols += 8;

	printk("Memory config: type %s, buswidth %d bits, banks %d, cols %d\n",
			miumemtypetbl[memtype], buswidth, banks, cols);

	regmap_field_read(priv->trcd, &trcd);
	regmap_field_read(priv->trp, &trp);
	regmap_field_read(priv->tras, &tras);
	regmap_field_read(priv->trcdmsb, &trcdmsb);
	regmap_field_read(priv->trpmsb, &trpmsb);

	printk("Timing: trcd %x, trp %x, tras %x, trcdmsb %x, trpmsb %x\n",
			trcd, trp, tras, trcdmsb, trpmsb);

#if 0
	printk("clk gen dump\n");
	for (int i = 0; i < 0x200; i += 4){
		unsigned int value = readw(CLKGEN + i);
		printf("miu dump: 0x%08x - 0x%04x\n", CLKGEN + i, value);

	}

	printk("clk mui dump\n");
	for (int i = 0; i < 0x800; i += 4){
		unsigned int value = readw(MIU_ANA + i);
		printf("miu dump: 0x%08x - 0x%04x\n", MIU_ANA + i, value);

	}
#endif

out:
	return ret;
}

static int mstar_miu_get_info(struct udevice *dev,
		struct ram_info *info)
{
	return 0;
}

static const struct ram_ops mstar_miu_ops = {
	.get_info = mstar_miu_get_info,
};

static const struct udevice_id mstar_miu_ids[] = {
	{ .compatible = "mstar,msc313-miu" },
	{ .compatible = "mstar,ssc8336-miu" },
	{ .compatible = "sstar,ssd210-miu" },
	{ /* sentinel */ }
};

U_BOOT_DRIVER(mstar_miu) = {
	.name = "mstar_miu",
	.id = UCLASS_RAM,
	.of_match = mstar_miu_ids,
	.ops = &mstar_miu_ops,
	.probe = mstar_miu_probe,
	.priv_auto = sizeof(struct mstar_miu_priv),
};

#endif
