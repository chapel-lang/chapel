/*
 * Copyright 2004-2020 Cray Inc.
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

// Some of the portable C implementations of these functions were taken from:
//   http://graphics.stanford.edu/~seander/bithacks.html
// Which is compiled by Sean Eron Anderson and has been placed under public
// domain
//
// Compiling with -DCHPL_BITOPS_C forces the native C version of each to be
// used when possible

#ifndef _chpl_bitops_h_
#define _chpl_bitops_h_

#include <assert.h>
#include <stdint.h>
#include <limits.h>

#include "chpl-comp-detect-macros.h"

// chpl_bitops_popcount_*
// ----------------------
// C implementations from Bithacks: 'Counting bits set, in parallel'
// With -O3 and no support for the popcount instruction, the C versions produce
// the same assembly as the builtin under clang
// Returns: number of bits set in the provided integer

static inline uint32_t chpl_bitops_popcount_32(unsigned int x) {
#if !defined(CHPL_BITOPS_C) && (RT_COMP_CC & (~RT_COMP_PGI))
  // - Assumes 'int's are 32bit
  // - gcc4.3's doesn't have support for popcnt and the like
  // - If -march isn't set I think the intrinsic has a pretty high chance of
  //   being slower than the below C (on OSX/gcc at least it seems to do a lazy
  //   dyld call).
  return __builtin_popcount(x);
#else
  x = x - ((x >> 1) & 0x55555555);
  x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
  return (((x + (x >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24;
#endif
}

static inline uint64_t chpl_bitops_popcount_64(unsigned long long x) {
#if !defined(CHPL_BITOPS_C) && (RT_COMP_CC & (~RT_COMP_PGI))
  // Assumes 'long long's are 64bit
  return __builtin_popcountll(x);
#else
#define T unsigned long long
  x = x - ((x >> 1) & (T)~(T)0/3);
  x = (x & (T)~(T)0/15*3) + ((x >> 2) & (T)~(T)0/15*3);
  x = (x + (x >> 4)) & (T)~(T)0/255*15;
  return (T)(x * ((T)~(T)0/255)) >> (sizeof(T) - 1) * CHAR_BIT;
#undef T
#endif
}


// chpl_bitops_clz_*
// -----------------
// C implementation from http://aggregate.org/MAGIC/#Leading%20Zero%20Count
// Returns: number of leading zeros in the provided integer

// TODO: find a better bit hack to do this? the popcount at the end makes it
//       roughly twice as slow as the others
static inline uint32_t chpl_bitops_clz_32(unsigned int x) {
#if !defined(CHPL_BITOPS_C) && (RT_COMP_CC & (~RT_COMP_PGI))
  // - Assumes 'int's are 32bit
  // - __builtin_clz(0) is undefined, return 32 when 0, these conditionals
  //   are pretty unfortunate, but I don't see good way around it at the moment
  //   What makes it even worse is most platforms will return the number of
  //   bits when clz(0) is called (when arch >= sse4/LZCNT is available)
  return x ? __builtin_clz(x) : 32;
#else
  // - This works by filling all but the leading 0s with 1s, then calling
  //   popcount and subtracting from the total bits, leaving us with the
  //   leading 0 count
  //        Example: x = 0x04000000
  //                 after shift     after or
  x |= (x >> 1);  //  0x02000000   0x06000000
  x |= (x >> 2);  //  0x01800000   0x07800000
  x |= (x >> 4);  //  0x00780000   0x07f80000
  x |= (x >> 8);  //  0x0007f800   0x07fff800
  x |= (x >> 16); //  0x000007ff   0x07ffffff
  return (32 - chpl_bitops_popcount_32(x));
#endif
}

static inline uint64_t chpl_bitops_clz_64(unsigned long long x) {
#if !defined(CHPL_BITOPS_C) && (RT_COMP_CC & (~RT_COMP_PGI))
  // - Assumes 'long long's are 64bit
  // - Same as above, __builtin_clzll(0) is undefined, return 64 when 0
  return x ? __builtin_clzll(x) : 64;
#else
  // - This works the same as for 32bits, we just need one more shift. See above
  //   for an example.
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  x |= (x >> 32);
  return (64 - chpl_bitops_popcount_64(x));
#endif
}


// chpl_bitops_ctz_*
// -----------------
// C implementation from Bithacks: 'Count the consecutive zero bits (trailing)
//                                   on the right with multiply and lookup'
// Returns: number of trailing zeros in the provided integer

// Lookup table for the C implementation
extern const uint8_t chpl_bitops_debruijn32[32];

static inline uint32_t chpl_bitops_ctz_32(unsigned int x) {
#if !defined(CHPL_BITOPS_C) && (RT_COMP_CC & (~RT_COMP_PGI))
  // - Assumes 'int's are 32bit
  // - __builtin_ctz(0) is undefined, return 0 when 0
  return x ? __builtin_ctz(x) : 0;
#else
  // This works through the combination of a few tricks:
  // - (x & -x) takes advantage of twos complement to leave us with the least
  //   significant bit set
  // - 0x077cb531 is a 32 bit de Bruijn sequence - when we multiply our bit by
  //   this, a unique pattern will be set in the 5 high bits of the result
  //   value, shifting this back down gives us the index into our lookup table.
  return chpl_bitops_debruijn32[((uint32_t)((x & -x) * 0x077CB531U)) >> 27];
#endif
}

// Lookup table for the C implementation
extern const uint8_t chpl_bitops_debruijn64[64];

static inline uint64_t chpl_bitops_ctz_64(unsigned long long x) {
#if !defined(CHPL_BITOPS_C) && (RT_COMP_CC & (~RT_COMP_PGI))
  // Assumes 'long long's are 64bit
  // __builtin_ctzll(0) is undefined, return 0 when 0
  return x ? __builtin_ctzll(x) : 0;
#else
  // - This works the same as the 32bit version, only with a de Bruijn sequence
  //   for 64 bits.
  return chpl_bitops_debruijn64[((uint64_t)((x & -x) * 0x0218A392CD3D5DBFU)) >> 58];
#endif
}


// chpl_bitops_parity_*
// --------------------
// C implementations from Bithacks: 'Compute parity of word with a multiply'
// Returns: 0 if an even number of bits are set
//          1 if an odd number of bits are set

static inline uint32_t chpl_bitops_parity_32(unsigned int x) {
#if !defined(CHPL_BITOPS_C) && (RT_COMP_CC & (~RT_COMP_PGI))
  // This will expand to (confirmed w/ GCC):
  // When`popcnt` is supported:    Otherwise:
  // asm {                         asm {
  //   popcnt eax, edi               mov   eax, edi
  //   and    eax, 1                 shr   edi, 16
  //   ret                           xor   eax, edi
  // }                               xor   al,  ah
  //                                 setnp al
  //                                 movzx eax, al
  //                                 ret
  //                               }
  return __builtin_parity(x);
#else
  x ^= x >> 1;
  x ^= x >> 2;
  x = (x & 0x11111111U) * 0x11111111U;
  return (x >> 28) & 1;
#endif
}

static inline uint64_t chpl_bitops_parity_64(unsigned long long x) {
#if !defined(CHPL_BITOPS_C) && (RT_COMP_CC & (~RT_COMP_PGI))
  return __builtin_parityll(x);
#else
  x ^= x >> 1;
  x ^= x >> 2;
  x = (x & 0x1111111111111111UL) * 0x1111111111111111UL;
  return (x >> 60) & 1;
#endif
}

// chpl_bitops_rotl_*
// ------------------
// See "Safe, Efficient, and Portable Rotate in C/C++" by John Regehr
//     http://blog.regehr.org/archives/1063
// These should be recognized by the backend C compiler and turned into a rol
//
// Returns: x rotated left by n

#define UI(size) uint##size##_t
#define CHPL_BITOPS_ROTL(size) \
static inline UI(size) chpl_bitops_rotl_##size(UI(size) x, UI(size) n) { \
  assert(n < size);                                                      \
  return (x << n) | (x >> (-n & (size - 1)));                            \
}

CHPL_BITOPS_ROTL(8)
CHPL_BITOPS_ROTL(16)
CHPL_BITOPS_ROTL(32)
CHPL_BITOPS_ROTL(64)

#undef CHPL_BITOPS_ROTL

//
// chpl_bitops_rotr_*
// ------------------
// See "Safe, Efficient, and Portable Rotate in C/C++" by John Regehr
//     http://blog.regehr.org/archives/1063
// These should be recognized by the backend C compiler and turned into a ror
//
// Returns: x rotated left by n

#define CHPL_BITOPS_ROTR(size) \
static inline UI(size) chpl_bitops_rotr_##size(UI(size) x, UI(size) n) { \
  assert(n < size);                                                      \
  return (x >> n) | (x << (-n & (size - 1)));                            \
}

CHPL_BITOPS_ROTR(8)
CHPL_BITOPS_ROTR(16)
CHPL_BITOPS_ROTR(32)
CHPL_BITOPS_ROTR(64)

#undef CHPL_BITOPS_ROTR
#undef UI
#endif // _chpl_bitops_h_
