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

#ifndef _chpl_util_h_
#define _chpl_util_h_

#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


////////////////////////////////////////
//
// alignment
//

// If we have a mask representing 2^n - 1,
// round an offset down to a multiple of 2^n.
static inline
uintptr_t round_down_to_mask(uintptr_t p, uintptr_t mask)
{
  return p - (p & mask);
}
static inline
unsigned char* round_down_to_mask_ptr(unsigned char* p, uintptr_t mask)
{
  return (unsigned char*) round_down_to_mask((uintptr_t)p, mask);
}

// If we have a mask representing 2^n - 1,
// round an offset up to a multiple of 2^n.
static inline
uintptr_t round_up_to_mask(uintptr_t p, uintptr_t mask)
{
  return (p + mask) & ~mask;
}
static inline
unsigned char* round_up_to_mask_ptr(unsigned char* p, uintptr_t mask)
{
  return (unsigned char*) round_up_to_mask((uintptr_t)p, mask);
}


////////////////////////////////////////
//
// snprintf() a K/M/G size
//

static inline
int chpl_snprintf_z_KMG(char* buf, int bufSize, size_t val) {
  const size_t GiB = (size_t) (1UL << 30);
  if (val >= GiB)
    return snprintf(buf, bufSize, "%zdG", val / GiB);

  const size_t MiB = (size_t) (1UL << 20);
  if (val >= MiB)
    return snprintf(buf, bufSize, "%zdM", val / MiB);

  const size_t KiB = (size_t) (1UL << 10);
  if (val >= KiB)
    return snprintf(buf, bufSize, "%zdK", val / KiB);

  return snprintf(buf, bufSize, "%zd", val);
}

static inline
int chpl_snprintf_f_KMG(char* buf, int bufSize, double val) {
  const double GiB = (double) (1UL << 30);
  if (val >= GiB)
    return snprintf(buf, bufSize, "%.1fG", val / GiB);

  const double MiB = (double) (1UL << 20);
  if (val >= MiB)
    return snprintf(buf, bufSize, "%.1fM", val / MiB);

  const double KiB = (double) (1UL << 10);
  if (val >= KiB)
    return snprintf(buf, bufSize, "%.1fK", val / KiB);

  return snprintf(buf, bufSize, "%.1f", val);
}


#endif // _chpl_util_h_
