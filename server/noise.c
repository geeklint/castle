#include "noise.h"
/*
    This file adapted from java code by Stefan Gustavson.
    Source: http://staffwww.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java
*/

#include <math.h> 

#include "mm.h"

static struct MMType * size = NULL;

/* Object state */
struct Noise{
    short perm[512];
    short permMod12[512];
};

/* Grad array */
static signed char GRAD[][2] = {
        {1,1}, {-1,1}, {1,-1}, {-1,-1}, {1,0}, {-1,0},
        {1,0}, {-1,0}, {0,1}, {0,-1}, {0,1}, {0,-1}};

/* Skew constants */
static const double F2 = // 0.5*(sqrt(3.0)-1.0)
        0.3660254037844385965883020617184229195117950439453125;
static const double G2 = // (3.0-sqrt(3.0))/6.0
        0.2113248654051871344705659794271923601627349853515625;
static const double F3 = // 1.0/3.0
        0.333333333333333314829616256247390992939472198486328125;
static const double G3 = // 1.0/6.0
        0.1666666666666666574148081281236954964697360992431640625;
static const double F4 = // (sqrt(5.0)-1.0)/4.0
        0.30901699437494745126286943559534847736358642578125;
static const double G4 = // (5.0-Math.sqrt(5.0))/20.0
        0.1381966011250105041963109897551476024091243743896484375;

/* Initialize new Noise */
struct Noise * noise_new(struct Random * rand){
    struct Noise * self;
    int i;
    
    if (!size){
        size = mm_new(sizeof(struct Noise));
    }
    
    if ((self = mm_alloc(size))){
        for (i = 0; i < 512; ++i){
            self->perm[i] = i;
        }
        random_shuffle(rand, self->perm, sizeof(short), 512);
        for (i = 0; i < 512; ++i){
            self->permMod12[i] = self->perm[i] % 12;
        }
    }
    return self;
}

/* Deinitialize Noise */
void noise_del(struct Noise * self){
    mm_free(self);
}

/* Dot product */
static double dot(double x1, double y1, double x2, double y2){
    return x1 * x2 + y1 * y2;
}

/* Get noise */
double noise_noise(struct Noise * self, double xin, double yin){
    double n0, n1, n2; // Noise contributions from the three corners
    // Skew the input space to determine which simplex cell we're in
    double s = (xin + yin) * F2; // Hairy factor for 2D
    int i = (int) floor(xin + s);
    int j = (int) floor(yin + s);
    double t = (i + j) * G2;
    double X0 = i - t; // Unskew the cell origin back to (x,y) space
    double Y0 = j - t;
    double x0 = xin - X0; // The x,y distances from the cell origin
    double y0 = yin - Y0;
    double x1, y1, x2, y2, t0, t1, t2;
    int ii, jj, gi0, gi1, gi2;
    // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
    int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
    if (x0 > y0){ // lower triangle, XY order: (0,0)->(1,0)->(1,1)
        i1=1;
        j1=0;
    } else {      // upper triangle, YX order: (0,0)->(0,1)->(1,1)
        i1=0;
        j1=1;
    }
    // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
    // c = (3-sqrt(3))/6
    x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    y1 = y0 - j1 + G2;
    x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
    y2 = y0 - 1.0 + 2.0 * G2;
    // Work out the hashed gradient indices of the three simplex corners
    ii = i & 255;
    jj = j & 255;
    gi0 = self->permMod12[ii + self->perm[jj]];
    gi1 = self->permMod12[ii + i1 + self->perm[jj + j1]];
    gi2 = self->permMod12[ii + 1 + self->perm[jj + 1]];
    // Calculate the contribution from the three corners
    t0 = 0.5 - x0 * x0 - y0 * y0;
    if (t0 < 0){
        n0 = 0.0;
    } else {
      t0 *= t0;
      n0 = t0 * t0 * dot(GRAD[gi0][0], GRAD[gi0][1], x0, y0);
    }
    t1 = 0.5 - x1 * x1 - y1 * y1;
    if (t1 < 0){
        n1 = 0.0;
    } else {
      t1 *= t1;
      n1 = t1 * t1 * dot(GRAD[gi1][0], GRAD[gi1][1], x1, y1);
    }
    t2 = 0.5 - x2 * x2 - y2 * y2;
    if (t2 < 0){
        n2 = 0.0;
    } else {
      t2 *= t2;
      n2 = t2 * t2 * dot(GRAD[gi2][0], GRAD[gi2][1], x2, y2);
    }
    // Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the interval [-1,1].
    return 70.0 * (n0 + n1 + n2);
}

