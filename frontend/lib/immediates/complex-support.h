/*
 * Copyright 2024-2025 Hewlett Packard Enterprise Development LP
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

#ifndef IMMEDIATES_COMPLEX_SQRT_H
#define IMMEDIATES_COMPLEX_SQRT_H

/*

 This header exists to work around a problem with complex square root being
 inaccurate on libc++ by using C functions to do the complex square root.

 */

struct complex64 {
  float r;
  float i;
};
struct complex128 {
  double r;
  double i;
};

struct complex64 complexSqrt64(struct complex64 x);
struct complex128 complexSqrt128(struct complex128 x);

#endif
