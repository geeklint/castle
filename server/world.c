
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
    struct Climate * temperature,
                   * humidity,
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
    	self->temperature = climate_new(random_random(random));
    	self->humidity = climate_new(random_random(random));
    	self->evil = climate_new(random_random(random));
    }
    return self;
}

/* Deinitialize World */
void world_del(struct World * self){
    mm_free(self);
}

static enum Biome get_biome(double humid, double temp){
	if (temp > .5 + humid){
		return BiomeDesert;
	} else if (temp > 1.5 - humid){
		return BiomeRainforest;
	} else if (temp < .5 - humid){
		return BiomeTundra;
	} else if (temp < humid - .5){
		return BiomeTaiga;
	} else if (temp > humid){
		if (temp > 1 - humid){
			return BiomePlains;
		} else {
			return BiomeForest;
		}
	} else if (temp > 1 - humid){
		return BiomeSwamp;
	} else {
		return BiomeSnow;
	}
}

/* Get stats for a point in the world */
void world_dot(struct World * self, struct WorldDot * dot, long x, long y){
	double altitude, temperature, humidity;
	
	dot->evil = climate_climate(self->evil, x, y);
	dot->altitude = altitude = terrain_terrain(
			self->altitude, x, y);
	if (altitude > 2./3.){
		dot->biome = BiomeMountain;
	} else if (altitude < 1./3.){
		dot->biome = BiomeOcean;
	} else {
		temperature = climate_climate(self->temperature, x, y);
		humidity = climate_climate(self->humidity, x, y);
		dot->biome = get_biome(humidity, temperature);
	}
}

/* Save world */
int world_save(struct World * self, FILE * file){
	return 0; // TODO
}

/* Load world */
struct World * world_load(FILE * file){
	return NULL; // TODO
}
