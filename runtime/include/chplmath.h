/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _CHPL_MATH_H_
#define _CHPL_MATH_H_

static inline float chpl_macro_INFINITY(void) {
  return INFINITY;
}

static inline float chpl_macro_NAN(void) {
  return NAN;
}

static inline int chpl_macro_double_isinf(double x) { return isinf(x); }
static inline int chpl_macro_float_isinf(float x) { return isinf(x); }
static inline int chpl_macro_double_isfinite(double x) { return isfinite(x); }
static inline int chpl_macro_float_isfinite(float x) { return isfinite(x); }
static inline int chpl_macro_double_isnan(double x) { return isnan(x); }
static inline int chpl_macro_float_isnan(float x) { return isnan(x); }
static inline int chpl_macro_double_signbit(double x) { return signbit(x); }
static inline int chpl_macro_float_signbit(float x) { return signbit(x); }

// 32-bit Bessel functions aren't available on all platforms. For cases where
// we know they're available use them since they should be faster, but in other
// cases default to using the 64-bit versions and casting.
#ifdef __linux__
static inline float chpl_float_j0(float x) { return j0f(x); }
static inline float chpl_float_j1(float x) { return j1f(x); }
static inline float chpl_float_jn(int n, float x) { return jnf(n, x); }
static inline float chpl_float_y0(float x) { return y0f(x); }
static inline float chpl_float_y1(float x) { return y1f(x); }
static inline float chpl_float_yn(int n, float x) { return ynf(n, x); }
#else
static inline float chpl_float_j0(float x) { return (float)j0(x); }
static inline float chpl_float_j1(float x) { return (float)j1(x); }
static inline float chpl_float_jn(int n, float x) { return (float)jn(n, x); }
static inline float chpl_float_y0(float x) { return (float)y0(x); }
static inline float chpl_float_y1(float x) { return (float)y1(x); }
static inline float chpl_float_yn(int n, float x) { return (float)yn(n, x); }
#endif

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
