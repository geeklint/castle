
#ifndef CLIMATE_H_
#define CLIMATE_H_

#include <stdint.h>

/* Object state */
struct Climate;

/* Initialize new Climate */
struct Climate * climate_new(uint32_t seed);

/* Deinitialize Climate */
void climate_del(struct Climate *);

/* Get climate */
double climate_climate(struct Climate *, double x, double y);

#endif /* CLIMATE_H_ */
