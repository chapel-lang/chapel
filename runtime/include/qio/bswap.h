/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

// bswap.h
//
// Get (or provide) definitions for htobe64
// ( in particular, htobe## htole## be##toh and le##toh for
//   widths of 16, 32, and 64).
// 
// DO NOT modify this file without testing on at least
//  GCC, PGI, Intel, Cray, and Mac OS X compilers!
#ifndef _BSWAP_H_
#define _BSWAP_H_

// On a BSD derived system (such as Mac OS X),
// sys_basic will define _USE_BSD and include sys/types.h,
// and between that and sys/param.h, a BSD system should
// give us these byte order functions.
// (A BSD-derived system might define them in sys/endian.h,
//  but that should be included by sys/types.h or sys/param.h)
#include "sys_basic.h"

#include <sys/param.h>

#ifdef __GLIBC__
// We know that compilers depending on GLIBC will supply the following two
// includes: endian.h and byteswap.h.
//
// endian.h should supply htobe64 etc since glibc 2.9
//    (commit May 2008, release Feb 2009)
// byteswap.h should supply bswap_16, bswap_32, and bswap_64 since 2.1.1
//    (commit Jun 1997, release Feb 1999).
//
// These supplied functions are macros in all GLIBCs.
// They are implemented in terms of e.g. __bswap_16, which may be
// a function or a macro.
//
// Unfortunately, when using GLIBC with a compiler other than GCC,
// such as PGI, Cray or Intel compilers, __bswap_64 will not be
// defined (even though htobe64 is).
// There is a check in glibc for #if defined __GNUC__ && __GNUC__ >= 2
// 
# include <endian.h>
# include <byteswap.h>

// Now, if GLIBC did not supply bswap_64, we know that __bswap_64 is missing
// and so htobe64/htole64/be64toh/le64toh are broken.
# ifndef bswap_64
#  define REDO_BSWAP_64
# endif
// With the PGI compiler, there is a version of byteswap.h with that
// check removed, so we always use the compatibility 64-bit
// methods with PGI.
# ifdef __PGI
#  define REDO_BSWAP_64
# endif

# ifdef REDO_BSWAP_64
#  ifdef bswap_64
#   undef bswap_64
#  endif
#  ifdef htobe64
#   undef htobe64
#  endif
#  ifdef htole64
#   undef htole64
#  endif
#  ifdef be64toh
#   undef be64toh
#  endif
#  ifdef le64toh
#   undef le64toh
#  endif
# endif

#endif // __GLIBC__

// Now, for each argument width, we will make sure that we
// have htobe## htole## be##toh and le##toh. We do that by
// checking for one of them. We assume that if one of that
// width exists, the rest do. If we don't have bswap_## already,
// we will define it so that we can define the the htobe...
// functions based on the machine's byte order.
//
// These default implementations are copied from glibc bits/byteswap.h
// and endian.h

#ifndef htobe16
# ifndef bswap_16
# define bswap_16(x) \
  ((uint16_t) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))
# endif
# if __BYTE_ORDER == __LITTLE_ENDIAN
#   define htobe16(x) bswap_16 (x)
#   define htole16(x) (x)
#   define be16toh(x) bswap_16 (x)
#   define le16toh(x) (x)
#  else // __BYTE_ORDER == __BIG_ENDIAN
#   define htobe16(x) (x)
#   define htole16(x) bswap_16 (x)
#   define be16toh(x) (x)
#   define le16toh(x) bswap_16 (x)
# endif // __BYTE_ORDER
#endif // ! htobe16

#ifndef htobe32
# ifndef bswap_32
#  define bswap_32(x) \
  ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) |               \
  (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))
# endif
# if __BYTE_ORDER == __LITTLE_ENDIAN
#   define htobe32(x) bswap_32 (x)
#   define htole32(x) (x)
#   define be32toh(x) bswap_32 (x)
#   define le32toh(x) (x)
# else // __BYTE_ORDER == __BIG_ENDIAN
#   define htobe32(x) (x)
#   define htole32(x) bswap_32 (x)
#   define be32toh(x) (x)
#   define le32toh(x) bswap_32 (x)
# endif // __BYTE_ORDER
#endif // ! htobe32

#ifndef htobe64
# ifndef bswap_64
#  define bswap_64(x) \
     (  (((x) & 0xff00000000000000ull) >> 56)                                 \
      | (((x) & 0x00ff000000000000ull) >> 40)                                 \
      | (((x) & 0x0000ff0000000000ull) >> 24)                                 \
      | (((x) & 0x000000ff00000000ull) >> 8)                                  \
      | (((x) & 0x00000000ff000000ull) << 8)                                  \
      | (((x) & 0x0000000000ff0000ull) << 24)                                 \
      | (((x) & 0x000000000000ff00ull) << 40)                                 \
      | (((x) & 0x00000000000000ffull) << 56))
# endif
# if __BYTE_ORDER == __LITTLE_ENDIAN
#   define htobe64(x) bswap_64 (x)
#   define htole64(x) (x)
#   define be64toh(x) bswap_64 (x)
#   define le64toh(x) (x)
#  else // __BYTE_ORDER == __BIG_ENDIAN
#   define htobe64(x) (x)
#   define htole64(x) bswap_64 (x)
#   define be64toh(x) (x)
#   define le64toh(x) bswap_64 (x)
#  endif // __BYTE_ORDER
#endif // ! htobe64

#endif // ! _BSWAP_H_
