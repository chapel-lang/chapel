/*
 * Copyright 2024 Hewlett Packard Enterprise Development LP
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

#include "complex-support.h"

#include <complex.h>

static double complex makeDoubleComplex(double re, double im) {
  // Some test environments don't have a working CMPLX
  // so use another way to set if needed.
#if defined(CMPLX)
  return CMPLX(re, im);
#else
#ifndef CHPL_DONT_USE_CMPLX_PTR_ALIASING
#define cmplx_re64(c) (((double *)&(c))[0])
#define cmplx_im64(c) (((double *)&(c))[1])
  double complex val;
  cmplx_re64(val) = re;
  cmplx_im64(val) = im;
  return val;
#else
  // This can generate bad values in the face of inf/nan values
  return re + im*_Complex_I;
#endif
#endif
}

static float complex makeFloatComplex(float re, float im) {
  // Some test environments don't have a working CMPLXF
  // so use another way to set if needed.
#if defined(CMPLXF)
  return CMPLXF(re, im);
#else
#ifndef CHPL_DONT_USE_CMPLX_PTR_ALIASING
#define cmplx_re32(c) (((float *)&(c))[0])
#define cmplx_im32(c) (((float *)&(c))[1])
  float complex val;
  cmplx_re32(val) = re;
  cmplx_im32(val) = im;
  return val;
#else
  // This can generate bad values in the face of inf/nan values
  return re + im*_Complex_I;
#endif
#endif
}

struct complex64 complexSqrt64(struct complex64 x) {
  float complex c = makeFloatComplex(x.r, x.i);
  float complex n = csqrtf(c);
  struct complex64 ret;
  ret.r = crealf(n);
  ret.i = cimagf(n);
  return ret;
}
struct complex128 complexSqrt128(struct complex128 x) {
  double complex c = makeDoubleComplex(x.r, x.i);
  double complex n = csqrt(c);
  struct complex128 ret;
  ret.r = creal(n);
  ret.i = cimag(n);
  return ret;
}
