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

struct complex64 complexSqrt64(struct complex64 x) {
  float complex c = CMPLXF(x.r, x.i);
  float complex n = csqrtf(c);
  struct complex64 ret;
  ret.r = crealf(n);
  ret.i = cimagf(n);
  return ret;
}
struct complex128 complexSqrt128(struct complex128 x) {
  double complex c = CMPLX(x.r, x.i);
  double complex n = csqrt(c);
  struct complex128 ret;
  ret.r = creal(n);
  ret.i = cimag(n);
  return ret;
}
