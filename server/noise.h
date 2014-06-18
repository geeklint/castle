
#include "random.h"

/* Object state */
struct Noise;

/* Initialize new Noise */
struct Noise * noise_new(struct Random *);

/* Deinitialize Noise */
void noise_del(struct Noise *);

/* Get noise */
double noise_noise(struct Noise *, double x, double y);
