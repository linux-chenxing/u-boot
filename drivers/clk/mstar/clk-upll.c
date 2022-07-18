// SPDX-License-Identifier: GPL-2.0

#include <common.h>
#include <dm.h>
#include <clk.h>
#include <clk-uclass.h>
#include <regmap.h>
#include <syscon.h>
#include <chenxingv7.h>
#include <linux/err.h>
#include <dt-bindings/clock/mstar.h>

#define REG_CONFIG0	0x0

static const struct reg_field pd_field = REG_FIELD(REG_CONFIG0, 1, 1);
static const struct reg_field enddisc_field = REG_FIELD(REG_CONFIG0, 4, 4);
static const struct reg_field enfrun_field = REG_FIELD(REG_CONFIG0, 5, 5);
static const struct reg_field enxtal_field = REG_FIELD(REG_CONFIG0, 7, 7);

struct mstar_upll_priv {
	struct regmap_field *pd;
};

static int mstar_upll_enable(struct clk *clk)
{
	struct mstar_upll_priv *priv = dev_get_priv(clk->dev);

	regmap_field_write(priv->pd, 0);

	return 0;
}

#ifndef CONFIG_SPL_BUILD
static int mstar_upll_disable(struct clk *clk)
{
	struct mstar_upll_priv *priv = dev_get_priv(clk->dev);

	regmap_field_write(priv->pd, 1);

	return 0;
}
#endif

static const struct clk_ops mstar_upll_ops = {
	.enable = mstar_upll_enable,
#ifndef CONFIG_SPL_BUILD
	.disable = mstar_upll_disable,
#endif
};

static int mstar_upll_probe(struct udevice *dev)
{
	struct mstar_upll_priv *priv = dev_get_priv(dev);
	int ret;
	struct regmap regmap;

	ret = regmap_init_mem_index(dev_ofnode(dev), &regmap, 0);
	if(ret)
		goto out;

#if 0
	#define UPLL0 0x1f284000
	printk("clk upll dump\n");
	for (int i = 0; i < 0x200; i += 4){
		unsigned int value = readw(UPLL0 + i);
		printf("upll dump: 0x%08x - 0x%04x\n", UPLL0 + i, value);

	}
#endif

	priv->pd = devm_regmap_field_alloc(dev, &regmap, pd_field);

out:
	return ret;
}

static const struct udevice_id mstar_upll_ids[] = {
	{ .compatible = "mstar,upll", },
	{ }
};

U_BOOT_DRIVER(mstar_upll) = {
	.name = "mstar_upll",
	.id = UCLASS_CLK,
	.of_match = mstar_upll_ids,
	.probe = mstar_upll_probe,
	.priv_auto = sizeof(struct mstar_upll_priv),
	.ops = &mstar_upll_ops,
	.flags = DM_FLAG_PRE_RELOC,
};
