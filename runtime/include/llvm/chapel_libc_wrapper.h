/*
 * Copyright 2004-2018 Cray Inc.
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

#include "clang_builtins_wrapper.h"
#include <complex.h>

static inline double ADD_WRAPPER_PREFIX(cabs)(_Complex double c) {
#if __FAST_MATH__
    double x = __builtin_creal(c);
    double y = __builtin_cimag(c);
    return __builtin_sqrt(x*x+y*y);
#else
    return cabs(c);
#endif
}


static inline float ADD_WRAPPER_PREFIX(cabsf)(_Complex float c) {
#if __FAST_MATH__
    double x = __builtin_crealf(c);
    double y = __builtin_cimagf(c);
    return __builtin_sqrtf(x*x+y*y);
#else
    return cabsf(c);
#endif
}

static inline long double ADD_WRAPPER_PREFIX(cabsl)(_Complex long double c) {
#if __FAST_MATH__
    double x = __builtin_creall(c);
    double y = __builtin_cimagl(c);
    return __builtin_sqrtl(x*x+y*y);
#else
    return cabsl(c);
#endif
}
