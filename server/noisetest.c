/*
 * noisetest.c
 *
 *  Created on: Jun 19, 2014
 *      Author: geeklint
 */

#ifdef BUILD_NOISETEST

#include <math.h>
#include <stdio.h>
#include <time.h>

#include "noise.h"

#define ITER 1000
#define COUNT 10000

int main(){
	struct Random * random;
	struct Noise * noise;
	union {
		double d;
		uint32_t n[2];
	} x, y;
	double min = INFINITY, max = -INFINITY, n;

	random = random_new(time(0));
	noise = noise_new(random_new(random_random(random)));
	for (int i = ITER; i; --i){
		printf("\r%d%%", (int) (100 * (1. - ((float) i) / ITER)));
		for (int j = COUNT; j; --j){
			x.n[0] = random_random(random);
			x.n[1] = random_random(random);
			y.n[0] = random_random(random);
			y.n[1] = random_random(random);
			n = noise_noise(noise, x.d, y.d);
			if (n < min){
				min = n;
			}
			if (n > max){
				max = n;
			}
		}
	}
	printf("\rDone.\nMin=%f\nMax=%f\n", min, max);
	return 0;
}

#endif /* BUILD_NOISETEST */
