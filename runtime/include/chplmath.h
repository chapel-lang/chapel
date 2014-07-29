#ifndef _CHPL_MATH_H_
#define _CHPL_MATH_H_

static inline double chpl_macro_INFINITY(void) {
  double x;
  *(unsigned long long int*) &x = 0x7ff0000000000000ULL;   // positive infinity
  return x;
}

static inline double chpl_macro_NAN(void) {
  double x;
  *(unsigned long long int*) &x = 0x7ff8000000000001ULL;   // quiet NaN
  return x;
}

static inline int chpl_macro_double_isinf(double x) { return isinf(x); }
static inline int chpl_macro_float_isinf(float x) { return isinf(x); }
static inline int chpl_macro_double_isfinite(double x) { return isfinite(x); }
static inline int chpl_macro_float_isfinite(float x) { return isfinite(x); }
static inline int chpl_macro_double_isnan(double x) { return isnan(x); }
static inline int chpl_macro_float_isnan(float x) { return isnan(x); }


#ifdef DEFINE_32_BIT_MATH_FNS
#define fabsf(x) (float)fabs(x)
#define cosf(x) (float)cos(x)
#define erff(x) (float)erf(x)
#define expf(x) (float)exp(x)
#define logf(x) (float)log(x)
#define sinf(x) (float)sin(x)
#define tanf(x) (float)tan(x)
#define acosf(x) (float)acos(x)
#define asinf(x) (float)asin(x)
#define atanf(x) (float)atan(x)
#define cbrtf(x) (float)cbrt(x)
#define ceilf(x) (float)ceil(x)
#define coshf(x) (float)cosh(x)
#define erfcf(x) (float)erfc(x)
#define exp2f(x) (float)exp2(x)
#define log2f(x) (float)log2(x)
#define rintf(x) (float)rint(x)
#define sinhf(x) (float)sinh(x)
#define tanhf(x) (float)tanh(x)
#define sqrtf(x) (float)sqrt(x)
#define lgammaf(x) (float)lgamma(x)
#define acoshf(x) (float)acosh(x)
#define asinhf(x) (float)asinh(x)
#define atan2f(x,y) (float)atan2(x,y)
#define atanhf(x) (float)atanh(x)
#define expm1f(x) (float)expm1(x)
#define floorf(x) (float)floor(x)
#define log10f(x) (float)log10(x)
#define log1pf(x) (float)log1p(x)
#define roundf(x) (float)round(x)
#define truncf(x) (float)trunc(x)
#define nearbyintf(x) (float)nearbyint(x)
#define tgammaf(x) (float)tgamma(x)
#endif

#endif
