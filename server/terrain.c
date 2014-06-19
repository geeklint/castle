
#include "terrain.h"

#include <math.h>

#include "mm.h"
#include "noise.h"

static struct MMType * size = NULL;

/* Object state */
struct Terrain{
	unsigned int levels;
    struct Noise * noise;
    struct Terrain * micro;
};

/* Initialize new Terrain */
struct Terrain * terrain_new(uint32_t seed, unsigned int levels){
    struct Terrain * self;
    int rotate;
    
    if (!size){
        size = mm_new(sizeof(struct Terrain));
    }
    
    if ((self = mm_alloc(size))){
        self->noise = noise_new(random_new(seed));
        self->levels = levels;
        if (levels){
		    rotate = seed & 0x3f;
		    self->micro = terrain_new(
		    	(seed >> rotate) | (seed << (32 - rotate)),
		    	levels - 1);
		} else {
			self->micro = NULL;
		}
    }
    return self;
}

/* Deinitialize Terrain */
void terrain_del(struct Terrain * self){
	noise_del(self->noise);
	if (self->micro){
		terrain_del(self->micro);
	}
    mm_free(self);
}

/* Get value */
double terrain_terrain(struct Terrain * self, double x, double y){
	double u, f;
	double min, mode, max, span;
	
	f = pow(10, self->levels);
	u = noise_noise(self->noise, x / f, y / f);
	if (!self->micro){
		return u;
	} else {
		min = (u + 1.5) / 4;
		mode = (u + 1) / 2;
		max = (u + 1) / 4;
		span = max - min;
		u = terrain_terrain(self->micro, x, y);
		f = (mode - min) / span;
		if (u < f){
			return min + sqrt(u * span * (mode - min));
		} else {
			return max - sqrt((1 - u) * span * (max - mode));
		}
	}
}
