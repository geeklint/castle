
#include "distribution.h"

#include <math.h>

/* Change bounds of distribution */
double distribution_resize(double original, double min, double max){
	return (max - min) * original + min;
}

/* Reset bounds of distribution */
double distribution_normalize(double original, double min, double max){
	return (original - min) / (max - min);
}

/* Triangular distribution */
double distribution_triangle(double original, double mode){
	if (original < mode){
		return sqrt(original * mode);
	} else {
		return 1 - sqrt((1 - original) * (1 - mode));
	}
}

/* Inverse triangular */
double distribution_itriangle(double original, double imode){
	original = fmod(original + 1, 1);
	original = distribution_triangle(original, imode);
	original = fmod(original + 1, 1);
	return original;
}

