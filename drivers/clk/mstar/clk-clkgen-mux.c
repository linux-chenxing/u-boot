// SPDX-License-Identifier: GPL-2.0

#include <common.h>
#include <dm.h>
#include <clk-uclass.h>
#include <regmap.h>
#include <chenxingv7.h>
#include <dt-bindings/clock/mstar.h>

#if CONFIG_IS_ENABLED(OF_PLATDATA)
#include <dt-structs.h>
#endif

#define MAX_OUTPUTS 2

struct mstar_clkgen_mux_plat {
#if CONFIG_IS_ENABLED(OF_PLATDATA)
	struct dtd_mstar_clkgen_mux dtplat;
#endif
	int num_outputs;
	u32 gate_shifts[MAX_OUTPUTS];
	u32 mux_shifts[MAX_OUTPUTS];
	u32 mux_widths[MAX_OUTPUTS];
	u32 deglitches[MAX_OUTPUTS];
};

struct mstar_clkgen_mux_priv {
	struct regmap *regmap;
	struct clk_bulk parents;
	struct clk *parent[MAX_OUTPUTS];
};

static ulong mstar_clkgen_mux_get_rate(struct clk *clk)
{
	struct mstar_clkgen_mux_priv *priv = dev_get_priv(clk->dev);

	return clk_get_rate(priv->parent[clk->id]);
}

static int mstar_clkgen_set_parent(struct clk *clk, struct clk *parent)
{
	const struct mstar_clkgen_mux_plat *plat = dev_get_plat(clk->dev);
	struct mstar_clkgen_mux_priv *priv = dev_get_priv(clk->dev);
	int clkidx;
	u32 muxshift = plat->mux_shifts[clk->id], muxwidth = plat->mux_widths[clk->id];
	uint mask = GENMASK(muxshift + muxwidth, muxshift);

	for (clkidx = 0; clkidx < priv->parents.count; clkidx++){
		printf("checking %p %p\n", priv->parents.clks[clkidx].dev, parent->dev);
		if(priv->parents.clks[clkidx].dev == parent->dev){
			printk("found parent at %d\n", clkidx);
			goto setparent;
		}
	}

	return -EINVAL;

setparent:
	clk_enable(&priv->parents.clks[clkidx]);
	priv->parent[clk->id] = &priv->parents.clks[clkidx];
	regmap_update_bits(priv->regmap, 0, mask, clkidx << muxshift);

	return 0;
}

static int mstar_clkgen_mux_enable(struct clk *clk)
{
	const struct mstar_clkgen_mux_plat *plat = dev_get_plat(clk->dev);
	struct mstar_clkgen_mux_priv *priv = dev_get_priv(clk->dev);
	uint mask = BIT(plat->gate_shifts[clk->id]);

	regmap_update_bits(priv->regmap, 0, mask, 0);

	return 0;
}

#ifndef CONFIG_SPL_BUILD
static int mstar_clkgen_mux_disable(struct clk *clk)
{
	const struct mstar_clkgen_mux_plat *plat = dev_get_plat(clk->dev);
	struct mstar_clkgen_mux_priv *priv = dev_get_priv(clk->dev);
	uint mask = BIT(plat->gate_shifts[clk->id]);

	regmap_update_bits(priv->regmap, 0, mask, mask);

	return 0;
}
#endif

const struct clk_ops mstar_clkgen_mux_ops = {
	.set_parent = mstar_clkgen_set_parent,
	.enable = mstar_clkgen_mux_enable,
	.get_rate = mstar_clkgen_mux_get_rate,
#ifndef CONFIG_SPL_BUILD
	.disable = mstar_clkgen_mux_disable,
#endif
};

#if !CONFIG_IS_ENABLED(OF_PLATDATA)
static int mstar_clkgen_mux_of_to_plat(struct udevice *dev)
{
	struct mstar_clkgen_mux_plat *plat = dev_get_plat(dev);
	struct clk clk;
	int ret;

	plat->num_outputs = dev_read_string_count(dev, MSTAR_CLKGEN_MUX_OUTPUT_NAMES);

	if(plat->num_outputs <= 0 || plat->num_outputs > MAX_OUTPUTS){
		ret = -EINVAL;
		goto out;
	}

	ret = dev_read_u32_array(dev, MSTAR_CLKGEN_MUX_GATE_SHIFTS,
				plat->gate_shifts, plat->num_outputs);
	if(ret)
		goto out;

	ret = dev_read_u32_array(dev, MSTAR_CLKGEN_MUX_MUX_SHIFTS,
				plat->mux_shifts, plat->num_outputs);
	if(ret)
		goto out;

	ret = dev_read_u32_array(dev, MSTAR_CLKGEN_MUX_MUX_WIDTHS,
				plat->mux_widths, plat->num_outputs);
	if(ret)
		goto out;

	memset(plat->deglitches, 0xff, sizeof(plat->deglitches));
	dev_read_u32_array(dev, MSTAR_CLKGEN_MUX_DEGLITCHES,
			plat->deglitches, plat->num_outputs);
out:
	return ret;
}
#else
static int mstar_clkgen_mux_of_to_plat(struct udevice *dev) {
	return 0;
}
#endif

#define CLKGEN_UART_UART0MUX_MASK	(BIT(3) | BIT(2))

static int mstar_clkgen_mux_probe(struct udevice *dev)
{
	const struct mstar_clkgen_mux_plat *plat = dev_get_plat(dev);
	struct mstar_clkgen_mux_priv *priv = dev_get_priv(dev);
	int ret, i;

#if CONFIG_IS_ENABLED(OF_PLATDATA)
	ret = regmap_init_mem_plat(dev, &plat->dtplat.reg[0], 1, &priv->regmap);
	if (ret)
		goto out;

	priv->parents.count = ARRAY_SIZE(plat->dtplat.clocks);
	priv->parents.clks = kcalloc(priv->parents.count,
			sizeof(struct clk), GFP_KERNEL);
	if (!priv->parents.clks) {
		ret = -ENOMEM;
		goto out;
	}

	for (i = 0; i < priv->parents.count; i++) {
		ret = clk_get_by_phandle(dev, &plat->dtplat.clocks[i], &priv->parents.clks[i]);
		if (ret)
			goto out;
	}
#else
	ret = regmap_init_mem_index(dev_ofnode(dev), &priv->regmap, 0);
	if (ret)
		goto out;

	ret = clk_get_bulk(dev, &priv->parents);
	if (ret)
		goto out;
#endif

	/* Find the current parent for each mux output */
	for(i = 0; i < plat->num_outputs; i++) {
		uint mask = GENMASK(plat->mux_shifts[i] + plat->mux_widths[i], plat->mux_shifts[i]);
		unsigned int val;

		regmap_read(priv->regmap, 0, &val);
		val &= mask;
		val = val >> plat->mux_shifts[i];

		priv->parent[i] = &priv->parents.clks[val];
	}

	/*uint uartclkgen;
	regmap_read(priv->regmap, 0, &uartclkgen);
	uartclkgen &= ~CLKGEN_UART_UART0MUX_MASK;
	regmap_write(priv->regmap, 0, uartclkgen);*/

out:
	return ret;
}

static const struct udevice_id mstar_clkgen_mux_ids[] = {
	{ .compatible = "mstar,clkgen_mux", },
	{ }
};

U_BOOT_DRIVER(mstar_clkgen_mux) = {
	.name		= "mstar_clkgen_mux",
	.id		= UCLASS_CLK,
	.of_match	= mstar_clkgen_mux_ids,
	.of_to_plat	=  mstar_clkgen_mux_of_to_plat,
	.plat_auto	= sizeof(struct mstar_clkgen_mux_plat),
	.probe		= mstar_clkgen_mux_probe,
	.priv_auto	= sizeof(struct mstar_clkgen_mux_priv),
	.ops		= &mstar_clkgen_mux_ops,
	.flags		= DM_FLAG_PRE_RELOC,
};
