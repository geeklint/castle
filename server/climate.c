
#include "climate.h"

#include "mm.h"
#include "noise.h"

static struct MMType * size = NULL;

/* Object state */
struct Climate {
    struct Noise * mega;
    struct Noise * macro;
};

/* Initialize new Climate */
struct Climate * climate_new(uint32_t seed){
    struct Climate * self;
    
    if (!size){
        size = mm_new(sizeof(struct Climate));
    }
    
    if ((self = mm_alloc(size))){
        self->mega = noise_new(seed);
        self->macro = noise_new(~seed);
    }
    return self;
}

/* Deinitialize Climate */
void climate_del(struct Climate * self){
	noise_del(self->mega);
	noise_del(self->macro);
    mm_free(self);
}

/* Get climate */
double climate_climate(struct Climate * self, double x, double y){
	double ue, ua;

	ue = noise_noise(self->mega, x / 1000, y / 1000);
	ua = noise_noise(self->macro, x / 100, y / 100);
	return .1 * ua + .9 * ue;
}
