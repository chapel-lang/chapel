/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#ifdef __cplusplus
extern "C" {
#endif

MAYBE_GPU static inline int chpl_macro_double_isinf(double x) { return isinf(x); }
MAYBE_GPU static inline int chpl_macro_float_isinf(float x) { return isinf(x); }
MAYBE_GPU static inline int chpl_macro_double_isfinite(double x) { return isfinite(x); }
MAYBE_GPU static inline int chpl_macro_float_isfinite(float x) { return isfinite(x); }
MAYBE_GPU static inline int chpl_macro_double_isnan(double x) { return isnan(x); }
MAYBE_GPU static inline int chpl_macro_float_isnan(float x) { return isnan(x); }
MAYBE_GPU static inline int chpl_macro_double_signbit(double x) { return signbit(x); }
MAYBE_GPU static inline int chpl_macro_float_signbit(float x) { return signbit(x); }


#ifdef __cplusplus
// workaround for C++ lacking C99 complex support
MAYBE_GPU static inline double chpl_creal(_complex128 x) { return __builtin_creal(x); }
MAYBE_GPU static inline float  chpl_crealf(_complex64 x) { return __builtin_crealf(x); }
MAYBE_GPU static inline _imag64 chpl_cimag(_complex128 x) { return __builtin_cimag(x); }
MAYBE_GPU static inline _imag32 chpl_cimagf(_complex64 x) { return __builtin_cimagf(x); }
#else
MAYBE_GPU static inline double chpl_creal(_complex128 x) { return creal(x); }
MAYBE_GPU static inline float  chpl_crealf(_complex64 x) { return crealf(x); }
MAYBE_GPU static inline _imag64 chpl_cimag(_complex128 x) { return cimag(x); }
MAYBE_GPU static inline _imag32 chpl_cimagf(_complex64 x) { return cimagf(x); }
#endif

MAYBE_GPU static inline double chpl_sqrt64(double x) { return sqrt(x);  }
MAYBE_GPU static inline float  chpl_sqrt32(float x)  { return sqrtf(x); }
MAYBE_GPU static inline double chpl_fabs64(double x) { return fabs(x);  }
MAYBE_GPU static inline float  chpl_fabs32(float x)  { return fabsf(x); }

// 32-bit Bessel functions aren't available on all platforms. For cases where
// we know they're available use them since they should be faster, but in other
// cases default to using the 64-bit versions and casting.
#ifdef __linux__
MAYBE_GPU static inline float chpl_float_j0(float x) { return j0f(x); }
MAYBE_GPU static inline float chpl_float_j1(float x) { return j1f(x); }
MAYBE_GPU static inline float chpl_float_jn(int n, float x) { return jnf(n, x); }
MAYBE_GPU static inline float chpl_float_y0(float x) { return y0f(x); }
MAYBE_GPU static inline float chpl_float_y1(float x) { return y1f(x); }
MAYBE_GPU static inline float chpl_float_yn(int n, float x) { return ynf(n, x); }
#else
MAYBE_GPU static inline float chpl_float_j0(float x) { return (float)j0(x); }
MAYBE_GPU static inline float chpl_float_j1(float x) { return (float)j1(x); }
MAYBE_GPU static inline float chpl_float_jn(int n, float x) { return (float)jn(n, x); }
MAYBE_GPU static inline float chpl_float_y0(float x) { return (float)y0(x); }
MAYBE_GPU static inline float chpl_float_y1(float x) { return (float)y1(x); }
MAYBE_GPU static inline float chpl_float_yn(int n, float x) { return (float)yn(n, x); }
#endif

#ifdef __cplusplus
}
#endif

#endif
