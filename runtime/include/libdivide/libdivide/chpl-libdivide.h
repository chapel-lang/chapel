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

#ifndef _CHPL_LIBDIVIDE_H_
#define _CHPL_LIBDIVIDE_H_

#include <stdint.h>

#include "libdivide.h"


typedef struct libdivide_s32_t chpl_libdiv_s32_t;
typedef struct libdivide_u32_t chpl_libdiv_u32_t;
typedef struct libdivide_s64_t chpl_libdiv_s64_t;
typedef struct libdivide_u64_t chpl_libdiv_u64_t;


static inline
void chpl_libdiv_s32_gen(chpl_libdiv_s32_t* divToken,
                         int32_t divisor) {
  *divToken = libdivide_s32_gen(divisor);
}

static inline
void chpl_libdiv_u32_gen(chpl_libdiv_u32_t* divToken,
                         uint32_t divisor) {
  *divToken = libdivide_u32_gen(divisor);
}

static inline
void chpl_libdiv_s64_gen(chpl_libdiv_s64_t* divToken,
                         int64_t divisor) {
  *divToken = libdivide_s64_gen(divisor);
}

static inline
void chpl_libdiv_u64_gen(chpl_libdiv_u64_t* divToken,
                         uint64_t divisor) {
  *divToken = libdivide_u64_gen(divisor);
}


static inline
int32_t chpl_libdiv_s32_do(int32_t dividend,
                           const chpl_libdiv_s32_t* divToken) {
  return libdivide_s32_do(dividend, divToken);
}

static inline
int32_t chpl_libdiv_u32_do(uint32_t dividend,
                           const chpl_libdiv_u32_t* divToken) {
  return libdivide_u32_do(dividend, divToken);
}

static inline
int64_t chpl_libdiv_s64_do(int64_t dividend,
                           const chpl_libdiv_s64_t* divToken) {
  return libdivide_s64_do(dividend, divToken);
}

static inline
int64_t chpl_libdiv_u64_do(uint64_t dividend,
                           const chpl_libdiv_u64_t* divToken) {
  return libdivide_u64_do(dividend, divToken);
}

#endif
