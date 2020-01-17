/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef _CHPLMATH_H_
#define _CHPLMATH_H_

/* Wrappers for some math functions that aren't available before C++11.
   This header and these functions can be removed once we assume C++11. */

#if defined __cplusplus && __cplusplus >= 201103L
#include <cmath>
static inline bool chpl_signbit(double val) { return std::signbit(val); }
static inline bool chpl_isinf(double val) { return std::isinf(val); }
static inline bool chpl_isfinite(double val) { return std::isfinite(val); }
#else
#include <math.h>
static inline bool chpl_signbit(double val) { return signbit(val); }
static inline bool chpl_isinf(double val) { return isinf(val); }
static inline bool chpl_isfinite(double val) { return isfinite(val); }
#endif

#endif
