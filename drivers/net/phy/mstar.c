// SPDX-License-Identifier: GPL-2.0+
/*
 */

#include <config.h>
#include <common.h>
#include <phy.h>
#include <dm.h>
#include <fdt_support.h>
#include <syscon.h>
#include <chenxingv7.h>
#include <malloc.h>
#include <regmap.h>
#include <linux/err.h>

#include "emac.h"

DECLARE_GLOBAL_DATA_PTR;

#define MSC313_PHY_MASK 0xffffffff

/* Bank 0 */
#define BANK0		0x0
/* Bank 1 */
#define BANK1		0x200
#define REG_ANARST	(BANK1 + 0x8)
#define REG_ADCCLKSEL	(BANK1 + 0x74)
#define REG_REF		(BANK1 + 0x174)
#define REG_ADCPL	(BANK1 + 0x198)
#define REG_LDO		(BANK1 + 0x1f8)
/* Bank 2 */
#define BANK2		0x400
#define REG_200GAT	(BANK2 + 0x60)
#define REG_TX1		(BANK2 + 0x74)
#define REG_CLKOADCSEL	(BANK2 + 0x114)
#define REG_SADC	(BANK2 + 0x140)
#define REG_100GAT	(BANK2 + 0x188)
#define REG_TX2		(BANK2 + 0x1e0)

static const struct reg_field anarst = REG_FIELD(REG_ANARST, 5, 5);
static const struct reg_field sadcpd = REG_FIELD(REG_SADC, 12, 13);
static const struct reg_field adcplpd = REG_FIELD(REG_ADCPL, 4, 4);
static const struct reg_field refpd = REG_FIELD(REG_REF, 14, 15);
static const struct reg_field txpd1 = REG_FIELD(REG_TX1, 0, 7);
static const struct reg_field txpd2 = REG_FIELD(REG_TX2, 8, 15);
static const struct reg_field clkoadcsel = REG_FIELD(REG_CLKOADCSEL, 0, 7);
static const struct reg_field adcclksel = REG_FIELD(REG_ADCCLKSEL, 8, 8);
static const struct reg_field hundredgat = REG_FIELD(REG_100GAT, 14, 14);
static const struct reg_field twohundredgat = REG_FIELD(REG_200GAT, 4, 4);

struct mstar_phy_priv {
	struct regmap *phyana;
	struct regmap_field *anarst;
	struct regmap_field *sadcpd;
	struct regmap_field *adcplpd;
	struct regmap_field *refpd;
	struct regmap_field *txpd1;
	struct regmap_field *txpd2;
	struct regmap_field *clkoadcsel;
	struct regmap_field *adcclksel;
	struct regmap_field *hundredgat;
	struct regmap_field *twohundredgat;
};

static void msc313e_powerup(struct mstar_phy_priv *priv){
	printf("Doing phy power up\n");

	regmap_field_write(priv->anarst, 1);
	mdelay(100);
	regmap_field_write(priv->anarst, 0);
	mdelay(100);

	/* "Power-on LDO" */
	regmap_write(priv->phyana, REG_LDO, 0x0000);
	/* "Power-on SADC" */
	regmap_field_write(priv->sadcpd, 0);
        /* "Power-on ADCPL" */
	regmap_field_write(priv->adcplpd, 0);
        /* "Power-on REF" */
	regmap_field_write(priv->refpd, 0);
        /* "Power-on TX" */
	regmap_field_write(priv->txpd1, 0);
        /* "Power-on TX" */
	regmap_field_write(priv->txpd2, 0);
        /* "CLKO_ADC_SEL" */
	regmap_field_write(priv->clkoadcsel, 1);
	/* "reg_adc_clk_select" */
	regmap_field_write(priv->adcclksel, 1);
	/* "100gat" */
	regmap_field_write(priv->hundredgat, 0);
	/* "200gat" */
	regmap_field_write(priv->twohundredgat, 0);
}

static int mstar_phy_config(struct phy_device *phydev)
{
	struct mstar_phy_priv *priv;
	ofnode phy_node, phyana_node;
	u32 phandle;
	int ret;

	printf("phy power up\n");

	phy_node = phy_get_ofnode(phydev);
	if (!ofnode_valid(phy_node)){
		ret = -EINVAL;
		goto out;
	}

	ret = ofnode_read_u32(phy_node, "mstar,phyana", &phandle);
	if (ret)
		goto out;

	phyana_node = ofnode_get_by_phandle(phandle);
	if (!ofnode_valid(phyana_node)) {
		ret = -ENODEV;
		goto out;
	}

	priv = malloc(sizeof(*priv));
	if (!priv){
		ret = -ENOMEM;
		goto out;
	}

	phydev->priv = priv;
	priv->phyana = syscon_node_to_regmap(phyana_node);
	if(IS_ERR(priv->phyana)){
		ret = PTR_ERR(priv->phyana);
		goto out;
	}

	printf("%x(%x) \n", priv->phyana->ranges[0].start, priv->phyana->ranges[0].size);

	priv->anarst = regmap_field_alloc(priv->phyana, anarst);
	priv->sadcpd = regmap_field_alloc(priv->phyana, sadcpd);
	priv->adcplpd = regmap_field_alloc(priv->phyana, adcplpd);
	priv->refpd = regmap_field_alloc(priv->phyana, refpd);
	priv->txpd1 = regmap_field_alloc(priv->phyana, txpd1);
	priv->txpd2 = regmap_field_alloc(priv->phyana, txpd2);
	priv->clkoadcsel = regmap_field_alloc(priv->phyana, clkoadcsel);
	priv->adcclksel = regmap_field_alloc(priv->phyana, adcclksel);
	priv->hundredgat = regmap_field_alloc(priv->phyana, hundredgat);
	priv->twohundredgat = regmap_field_alloc(priv->phyana, twohundredgat);

	switch(mstar_chiptype()){
		case CHIPTYPE_MSC313:
			//emacclocks();
			emac_patches();
			emacphypowerup_msc313();
			break;
		case CHIPTYPE_MSC313E:
		case CHIPTYPE_MSC313DC:
			//emacclocks();
			emac_patches();
			msc313e_powerup(priv);
			break;
		default:
			break;
	}

	genphy_config_aneg(phydev);
	genphy_restart_aneg(phydev);

out:
	return ret;
}

static struct phy_driver msc313_phy_driver = {
	.uid		= MSC313_PHY_ID,
	.mask		= 0xffffffff,
	.name		= "MStar/SigmaStar PHY",
	.features	= PHY_BASIC_FEATURES,
	.config		= mstar_phy_config,
	.startup	= genphy_startup,
	.shutdown	= genphy_shutdown,
};

static struct phy_driver msc313e_phy_driver = {
	.uid		= MSC313E_PHY_ID,
	.mask		= 0xffffffff,
	.name		= "MStar/SigmaStar PHY",
	.features	= PHY_BASIC_FEATURES,
	.config		= mstar_phy_config,
	.startup	= genphy_startup,
	.shutdown	= genphy_shutdown,
};

int phy_mstar_init(void)
{
	phy_register(&msc313_phy_driver);
	phy_register(&msc313e_phy_driver);
	return 0;
}
