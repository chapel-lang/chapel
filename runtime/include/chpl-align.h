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

#ifndef _chpl_align_h_
#define _chpl_align_h_

#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <limits.h>

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


#endif // _chpl_align_h_
