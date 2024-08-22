// Description: VLIDE: Variable-Length Incremental Differential Encoding 
// Copyright 2023-2024, Dan Bonachea and UC Regents
// Terms of use are as specified in license.txt

#ifndef _GASNET_VLIDE_H
#define _GASNET_VLIDE_H

// VLIDE: Variable-Length Incremental Differential Encoding 
// Used to incrementally compress a series of large but similar values into a smaller, variable-length format.
//
// Format of each encoded byte:
//   [1 bit: more bytes?] | [7-bit: value chunk]
//   value chunks emitted in big endian order
// 
// Differential value encoding properties:
//
//  delta: (curr - prev)        encoding   compress 2s-c encoding   notes
//                                size      ratio     sz, max      
//  -------------------------   ---------- ------  --------------  --------
//           delta == 0       : 1 zero byte 88% 
// 0      <= |delta| < 64     : 1 byte      88%   1 byte, 128      useful for irregular field update in adjacent structs
// 64     <= |delta| < 8KiB   : 2 byte      75%   2 byte, 32KiB       "
// 8KiB   <= |delta| < 1MB    : 3 byte      75%   3 byte, 8MiB     common for irregular updates to single large object
// 1MiB   <= |delta| < 128MiB : 4 byte      50%   4 byte, 2GiB        "
// 128MiB <= |delta| < 16GiB  : 5 byte      38%   5 byte, 512GiB   40-bit not enough for global max in either rep.
// 16GiB  <= |delta| < 2TiB   : 6 byte      25%   6 byte, 128TiB   48-bit more than enough for global max in each
//   
// Legend:
// * encoding size: Space consumed by the VLIDE encoding for the given delta
// * compress ratio: Data compression space savings relative to a naive 64-bit 2's complement representation
// * 2s-c encoding sz,max: The minimum bytes in 2's complement encoding to encompass the same delta, 
//                         and the max value that encoding can encompass. This assumes the 2's encoded
//                         length is conveyed using some other out-of-band mechanism

// VLIDE encoding/decoding macros
// 
// arguments:
//  
//  uint8_t *datap 
//    cursor into the encoding buffer, advanced by the macro
//    must be an lvalue of this type, with no side-effects
// 
//  intX prev, intX curr
//    expressions of type 64-bit integer or (u)intptr_t (32- or 64-bit),
//    where both arguments have the same type
//
//  word result
//    lvalue of type 64-bit integer or (32- or 64-bit) pointer
//    type width must match the encoded curr
//    DECODE writes the result here
//

#define GASNETI_VLIDE_ENCODE(datap, prev, curr) do {                    \
  gasneti_static_assert(sizeof(*(datap)) == 1);                         \
  gasneti_static_assert(sizeof(prev) == 8 || sizeof(prev) == sizeof(void*)); \
  gasneti_static_assert(sizeof(curr) == sizeof(prev));                  \
  uint64_t _diff = (uint64_t)(curr) - (uint64_t)(prev);                 \
  if (_diff + 64 < 128) { /* single chunk optimization */               \
    uint8_t _val = _diff & 0x7F;                                        \
    *(datap)++ = _val;                                                  \
  } else { /* multi-chunk */                                            \
    int _chunks;                                                        \
    { /* compute necessary 7-bit chunks for the encoding */             \
      uint64_t _cdiff = _diff;                                          \
      if (_cdiff & (1LLU << 63))/* negative: toggle bits  */            \
          _cdiff = ~_cdiff;                                             \
      _cdiff >>= 6; /* offset to manipulate chunk transition */         \
      for (_chunks = 1; _cdiff ; _chunks++) _cdiff >>= 7;               \
      gasneti_assert_uint(_chunks ,<=, 10); /* theoretical limit */     \
    }                                                                   \
    uint8_t * const _next_datap = (datap) + _chunks;                    \
    uint8_t *_p = _next_datap;                                          \
    for (int _i = 0; _i < _chunks; _i++) {                              \
      uint8_t const _val = (_diff & 0x7F) /* value */ |                 \
                          (_i ? 0x80 : 0) /* more */;                   \
      _diff >>= 7;  /* does NOT sign extend */                          \
      *--_p = _val;                                                     \
    }                                                                   \
    if (_diff) { /* diff was negative, but all remaining bits are 1 */  \
      gasneti_assert_uint(_diff & (_diff + 1),==, 0);                   \
    }                                                                   \
    gasneti_assert_ptr(_p ,==, (datap));                                \
    (datap) = _next_datap;                                              \
  }                                                                     \
} while (0)

#define GASNETI_VLIDE_DECODE(datap, prev, result) do {                    \
  gasneti_static_assert(sizeof(*(datap)) == 1);                           \
  gasneti_static_assert(sizeof(prev) == 8 || sizeof(prev) == sizeof(void*)); \
  gasneti_static_assert(sizeof(result) == 8 || sizeof(result) == sizeof(void*)); \
  uint8_t _b = *datap;                                                    \
  uint64_t _diff;                                                         \
  if ((_b & 0xC0) == 0) { /* single-chunk positive optimization */        \
    _diff = _b;                                                           \
    datap++;                                                              \
  } else if ((_b & 0x80) == 0) { /* single-chunk negative optimization */ \
    _diff = ((uint64_t)-1 << 7) | _b;                                     \
    datap++;                                                              \
  } else { /* multi-chunk */                                              \
    int _more;                                                            \
    if (_b & 0x40) _diff = (uint64_t)-1;                                  \
    else           _diff = 0;                                             \
    do {                                                                  \
      _diff <<= 7;                                                        \
      _more  = (_b & 0x80);                                               \
      _diff |= (_b & 0x7F);                                               \
      _b = *++datap; /* assumes we can safely read a trailing byte */     \
    } while (_more);                                                      \
  }                                                                       \
  if (sizeof(result) == 8) {                                              \
    *(uint64_t *)&(result) = _diff + (uint64_t)(prev);                    \
  } else { gasneti_assert_uint(sizeof(result) ,==, sizeof(uintptr_t));    \
    *(uintptr_t *)&(result) = (uintptr_t)(_diff + (uint64_t)(prev));      \
  }  \
} while (0)

// GASNETI_VLIDE_{EN,DE}CODE_TRIVIAL provide the same API as non-TRIVIAL variants
// But use a trivial 64-bit 2's complement encoding, for debugging purposes

#define GASNETI_VLIDE_ENCODE_TRIVIAL(datap, prev, curr) do {      \
  gasneti_static_assert(sizeof(*(datap)) == 1);                           \
  gasneti_static_assert(sizeof(prev) == 8 || sizeof(prev) == sizeof(void*)); \
  gasneti_static_assert(sizeof(curr) == sizeof(prev)); \
  if (sizeof(curr) == 8) { \
    uint64_t _curr = (curr); \
    memcpy((datap), &_curr, sizeof(_curr));                             \
  } else { gasneti_assert_uint(sizeof(curr) ,==, 4);      \
    uint32_t _curr = (curr); \
    memcpy((datap), &_curr, sizeof(_curr));                             \
  }  \
  (datap) += sizeof(curr);                                          \
} while (0)

#define GASNETI_VLIDE_DECODE_TRIVIAL(datap, prev, result) do {       \
  gasneti_static_assert(sizeof(*(datap)) == 1);                           \
  gasneti_static_assert(sizeof(prev) == 8 || sizeof(prev) == sizeof(void*)); \
  gasneti_static_assert(sizeof(result) == 8 || sizeof(result) == sizeof(void*)); \
  memcpy(&(result), (datap), sizeof(result));                               \
  (datap) += sizeof(result);                                           \
} while (0)  


#endif // _GASNET_VLIDE_H
