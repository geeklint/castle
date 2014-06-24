#ifndef NOISE_H_
#define NOISE_H_

#include <stdint.h>

/* Object state */
struct Noise;

/* Initialize new Noise */
struct Noise * noise_new(uint32_t seed);

/* Deinitialize Noise */
void noise_del(struct Noise *);

/* Get noise */
double noise_noise(struct Noise *, double x, double y);

#endif /* NOISE_H_ */
