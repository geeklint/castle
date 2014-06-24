
#include "terrain.h"

#include <math.h>

#include "distribution.h"
#include "mm.h"
#include "noise.h"

static struct MMType * size = NULL;

/* Object state */
struct Terrain{
    struct Noise * mega;
    struct Noise * macro;
    struct Noise * micro;
};

/* Initialize new Terrain */
struct Terrain * terrain_new(uint32_t seed){
    struct Terrain * self;
    int rotate;
    
    if (!size){
        size = mm_new(sizeof(struct Terrain));
    }
    
    if ((self = mm_alloc(size))){
        self->mega = noise_new(seed);
	    rotate = seed & 0x3f;
    	seed = (seed >> rotate) | (seed << (32 - rotate));
        self->macro = noise_new(seed);
	    rotate = seed & 0x3f;
    	seed = (seed >> rotate) | (seed << (32 - rotate));
        self->micro = noise_new(seed);
    }
    return self;
}

/* Deinitialize Terrain */
void terrain_del(struct Terrain * self){
	noise_del(self->mega);
	noise_del(self->macro);
	noise_del(self->micro);
    mm_free(self);
}

/* Get value */
double terrain_terrain(struct Terrain * self, double x, double y){
	double ue, ua, ui;
	
	ue = noise_noise(self->mega, x / 10000, y / 10000);
	ua = noise_noise(self->macro, x / 1000, y / 1000);
	ui = noise_noise(self->micro, x / 100, y / 100);
	ue = distribution_itriangle(ue, .5); // continents
	ua = distribution_triangle(ua, 0); // more ocean
	ui = distribution_triangle(ui, 0); // pointy mountains
	return .25 * ui + .25 * ua + .5 * ue;
}
