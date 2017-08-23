/*
 * Copyright 2004-2017 Cray Inc.
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

#if __FAST_MATH__
static inline double cabs(_Complex double c)
{
    double x = __builtin_creal(c);
    double y = __builtin_cimag(c);
    return __builtin_sqrt(x*x+y*y);
}
#endif

#if __FAST_MATH__
static inline float cabsf(_Complex float c)
{
    float x = __builtin_creal(c);
    float y = __builtin_cimag(c);
    return __builtin_sqrtf(x*x+y*y);
}
#endif

#if __FAST_MATH__
static inline long double cabsl(_Complex long double c)
{
    long double x = __builtin_creall(c);
    long double y = __builtin_cimagl(c);
    return __builtin_sqrtl(x*x+y*y);
}
#endif
