/*
 * debug.h
 *
 *  Created on: 19 Aug 2022
 *      Author: daniel
 */

#ifndef DRIVERS_DDR_MSTAR_DEBUG_H_
#define DRIVERS_DDR_MSTAR_DEBUG_H_

static inline int mstar_regmap_write(const struct regmap *map, uint offset, uint val)
{
#ifdef CONFIG_MSTAR_MIU_DEBUG
	int ret;
	uint original, readback;

	regmap_read(map, offset, &original);

	printf("regmap write 0x%08x -> 0x%08x (was 0x%08x)\n",
			(unsigned int) (map->ranges[0].start + offset),
			(unsigned int) val,
			(unsigned int) original);

	ret = regmap_write(map, offset, val);
	regmap_read(map, offset, &readback);

	if (readback != val)
		printf("regmap write wanted 0x%08x, got 0x%08x\n", val, readback);

	return ret;
#else
	return regmap_write(map, offset, val);
#endif
}

#ifdef CONFIG_MSTAR_MIU_DEBUG
struct regmap_field {
	struct regmap *regmap;
	unsigned int mask;
	/* lsb */
	unsigned int shift;
	unsigned int reg;
};

static int mstar_regmap_field_write(const struct regmap_field *field, unsigned int val)
{
	int ret;
	uint original, new;

	regmap_read(field->regmap, field->reg, &original);

	ret = regmap_field_write(field, val);

	regmap_read(field->regmap, field->reg, &new);

	printf("regmap update 0x%08x -> 0x%08x (was 0x%08x)\n",
			(unsigned int) (field->regmap->ranges[0].start + field->reg),
			(unsigned int) new,
			(unsigned int) original);

	return ret;
}
#else
static inline int mstar_regmap_field_write(struct regmap_field *field, unsigned int val)
{
	return regmap_field_write(field, val);
}
#endif

#endif /* DRIVERS_DDR_MSTAR_DEBUG_H_ */
