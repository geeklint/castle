#include <stdlib.h>
#include <stdint.h>

/* Object state */
struct Random;

/* Initialize new generator */
struct Random * random_new(uint32_t seed);

/* Get random number */
uint32_t random_random(struct Random *);

/* Get number in range */
int random_range(struct Random *, int min, int max);

/* Shuffle array */
void random_shuffle(struct Random *, void *, size_t block, size_t count);

/* Deinitialize generator */
void random_del(struct Random *);
