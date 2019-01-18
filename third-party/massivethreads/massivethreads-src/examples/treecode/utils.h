/* utils.h */

#ifndef UTILS_H
#define UTILS_H

#include <cmath>

// Memory allocation
void * xmalloc(size_t);

// Math
#define PI         3.14159265358979323846
#define TWO_PI     6.28318530717958647693
#define FOUR_PI   12.56637061435917295385
#define HALF_PI    1.57079632679489661923
#define FRTHRD_PI  4.18879020478639098462

double xrandom(double, double);

#endif /* UTILS_H */
