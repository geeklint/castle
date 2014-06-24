
#include "world.h"

#include <math.h>

#include "climate.h"
#include "mm.h"
#include "random.h"
#include "terrain.h"

static struct MMType * size = NULL;

/* Object state */
struct World{
    uint32_t seed;
    struct Terrain * altitude;
    struct Climate * climate,
                   * evil;
};

/* Initialize new World */
struct World * world_new(uint32_t seed){
    struct World * self;
    struct Random * random;
    
    if (!size){
        size = mm_new(sizeof(struct World));
    }
    
    if ((self = mm_alloc(size))){
        self->seed = seed;
    	random = random_new(seed);
    	self->altitude = terrain_new(random_random(random));
    	self->climate = climate_new(random_random(random));
    	self->evil = climate_new(random_random(random));
    	random_del(random);
    }
    return self;
}

/* Deinitialize World */
void world_del(struct World * self){
	terrain_del(self->altitude);
	climate_del(self->climate);
	climate_del(self->evil);
    mm_free(self);
}

static enum Biome get_biome(double altitude, double climate){
	enum Biome biomes[] = {BiomeRockyMountain, BiomePlains, BiomeDesert,
						 BiomeForestMountain, BiomeForest, BiomeSwamp};
	int offset;

	offset = 3 * (climate > .5);
	if (altitude > .75){
		return BiomeMountainTop;
	} else if (altitude > .625){
		return biomes[offset];
	} else if (altitude > .5){
		return biomes[offset + 1];
	} else if (altitude > .375){
		return biomes[offset + 2];
	} else if (altitude > .325){
		return BiomeBeach;
	} else {
		return BiomeOcean;
	}
}

/* Get stats for a point in the world */
void world_dot(struct World * self, struct WorldDot * dot, long x, long y){
	double altitude, climate;
	
	dot->evil = climate_climate(self->evil, x, y);
	dot->altitude = altitude = terrain_terrain(
			self->altitude, x, y);
	climate = climate_climate(self->climate, x, y);
	dot->biome = get_biome(altitude, climate);
}

/* Save world */
int world_save(struct World * self, FILE * file){
	return 0; // TODO
}

/* Load world */
struct World * world_load(FILE * file){
	return NULL; // TODO
}
