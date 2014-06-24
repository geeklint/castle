#ifdef BUILD_WORLDTEST

#include <stdlib.h>
#include <stdio.h>

#include "world.h"

#define SEED 1234
#define WIDTH 7000
#define HEIGHT 5000

int biomecolors[][3] = {
		{0, 0, 255},     // Ocean,
		{210, 180, 140}, // Beach,
		{255, 255, 255}, // MountainTop,
		{127, 127, 127}, // RockyMountain,
		{34, 139, 34},   // ForestMountain,
		{113, 188, 120}, // Plains,
		{34, 139, 34},   // Forest,
		{210, 180, 140}, // Desert,
		{103, 76, 71},   // Swamp
};

int main(){
	FILE * ppm;
	char buf[WIDTH * 3];
	struct WorldDot dot;
	struct World * world;

	ppm = fopen("world.ppm", "w");
	fprintf(ppm, "P6 %d %d 255\n", WIDTH, HEIGHT);
	
	world = world_new(SEED);
	
	for (int row = 0; row < HEIGHT; ++row){
		printf("\r%d%% ", 100 * row / HEIGHT);
		for (int col = 0; col < WIDTH; ++col){
			world_dot(world, &dot, col, row);
			buf[3 * col] = biomecolors[dot.biome][0] * dot.altitude;
			buf[3 * col + 1] = biomecolors[dot.biome][1] * dot.altitude;
			buf[3 * col + 2] = biomecolors[dot.biome][2] * dot.altitude;
		}
		fwrite(buf, 3, WIDTH, ppm);
	}
	printf("\rDone.\n");
	fclose(ppm);
	world_del(world);
	
	return 0;
}

#endif /* BUILD_WORLDTEST */
