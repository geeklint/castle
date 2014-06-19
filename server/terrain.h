
#include <stdint.h>

/* Object state */
struct Terrain;

/* Initialize new Terrain */
struct Terrain * terrain_new(uint32_t seed);

/* Deinitialize Terrain */
void terrain_del(struct Terrain *);

/* Get value */
double terrain_terrain(struct Terrain *, double x, double y);
