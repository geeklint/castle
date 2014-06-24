#ifndef DISTRIBUTION_H_
#define DISTRIBUTION_H_

/*
 * The functions in this file serve to skew distributions
 */

/* Change bounds of distribution */
double distribution_resize(double original, double min, double max);

/* Reset bounds of distribution */
double distribution_normalize(double original, double min, double max);

/* Triangular distribution */
double distribution_triangle(double original, double mode);

/* Inverse triangular */
double distribution_itriangle(double original, double imode);

#endif /* DISTRIBUTION_H_ */
