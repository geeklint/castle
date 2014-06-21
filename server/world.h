#ifndef WORLD_H_
#define WORLD_H_

#include <stdint.h>
#include <stdio.h>

enum Biome {
	BiomeOcean,
	BiomeMountain,
	BiomeDesert,
	BiomeRainforest,
	BiomePlains,
	BiomeForest,
	BiomeSwamp,
	BiomeSnow,
	BiomeTundra,
	BiomeTaiga
};

struct WorldDot {
	enum Biome biome;
	double altitude;
	unsigned long evil;
};

/* Object state */
struct World;

/* Initialize new World */
struct World * world_new(uint32_t seed);

/* Deinitialize World */
void world_del(struct World *);

/* Get stats for a point in the world */
void world_dot(struct World *, struct WorldDot *, long x, long y);

/* Save world */
int world_save(struct World *, FILE *);

/* Load world */
struct World * world_load(FILE *);

#endif /* WORLD_H_ */
