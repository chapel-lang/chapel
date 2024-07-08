/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2016 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2016 Intel Corporation. All rights reserved. */

#ifndef _PSMI_HELP_H
#define _PSMI_HELP_H
#include "psm_log.h"

/* XXX gcc only */
#define PSMI_INLINE(FN) \
	static inline FN

#ifndef PACK_SUFFIX
/* XXX gcc only */
#define PACK_SUFFIX __attribute__((packed))
#endif

#define PSMI_ALWAYS_INLINE(FN) \
	static __inline__ FN __attribute__((always_inline));  \
	static __inline__ FN

#define PSMI_NEVER_INLINE(FN)             \
	static FN __attribute__((noinline));  \
	static FN

#define _PPragma(x) _Pragma(x)

#define STRINGIFY(s)	_STRINGIFY(s)
#define _STRINGIFY(s)	#s
#define PSMI_CURLOC	__FILE__ ":" STRINGIFY(__LINE__)
#define psmi_assert_always_loc(x, curloc)				\
	do {								\
	if_pf(!(x)) {							\
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,	\
				"Assertion failure at %s: %s", curloc,	\
				STRINGIFY(x));				\
	} } while (0)

#define psmi_assert_always(x)  psmi_assert_always_loc(x, PSMI_CURLOC)

#ifdef PSM_DEBUG
#  define psmi_assert(x)	psmi_assert_always(x)
#  define PSMI_ASSERT_INITIALIZED() psmi_assert_always(psm3_isinitialized())
#else
#  define psmi_assert(x)
#  define PSMI_ASSERT_INITIALIZED()
#endif

#define PSMI_ERR_UNLESS_INITIALIZED(ep)					\
	do {								\
		if (!psm3_isinitialized()) {				\
			PSM2_LOG_MSG("leaving");				\
			return psm3_handle_error(ep, PSM2_INIT_NOT_INIT,	\
				"PSM3 has not been initialized");	\
	  }								\
	} while (0)

#define PSMI_CHECKMEM(err, mem)			\
	do {					\
		if ((mem) == NULL) {		\
			(err) = PSM2_NO_MEMORY;	\
			goto fail;		\
		}				\
	} while (0)

#define PSMI_CACHEALIGN	__attribute__((aligned(64)))

#ifdef min
#undef min
#endif
#define min(a, b) ((a) < (b) ? (a) : (b))

#ifdef max
#undef max
#endif
#define max(a, b) ((a) > (b) ? (a) : (b))

// macros taken fron IbAccess imath.h
/* round up value to align, align must be a power of 2 */
#ifndef ROUNDUPP2
#define ROUNDUPP2(val, align)   \
	(((uint32_t)(val) + (uint32_t)(align) - 1) & (~((uint32_t)(align)-1)))
#endif
/* force to use 64 bits in 32bit box */
#ifndef ROUNDUP64P2
#define ROUNDUP64P2(val, align)   \
	(((uint64_t)(val) + (uint64_t)(align) - 1) & (~((uint64_t)(align)-1)))
#endif

/* round up value to align, align can be any value, less efficient than ROUNDUPP2 */
#ifndef ROUNDUP
#define ROUNDUP(val, align) \
	((( ((uint32_t)(val)) + (uint32_t)(align) -1) / (align) ) * (align))
#endif

/* round down value to align, align must be a power of 2 */
#ifndef ROUNDDOWNP2
#define ROUNDDOWNP2(val, align) \
	(((uint32_t)(val)) & (~((uint32_t)(align)-1)))
#endif

/* round down value to align, align can be any value, less efficient than ROUNDDOWNP2 */
#ifndef ROUNDDOWN
#define ROUNDDOWN(val, align)   \
	((( ((uint32_t)(val))) / (align) ) * (align))
#endif

#define PSMI_NBITS_TO_MASK(NBITS)	((uint64_t)((1 << NBITS)-1))

/* given a pointer to a member of a structure, ret ptr to parent struct */
#define PARENT_STRUCT(ADDRESS, TYPE, MEMBER) \
	((TYPE *)((char *)(ADDRESS) - offsetof(TYPE, MEMBER))) 

/* how many entries are in a statically allocated table */
#define PSMI_HOWMANY(table) (sizeof(table)/sizeof(table[0]))


#define SEC_ULL	 1000000000ULL
#define MSEC_ULL 1000000ULL
#define USEC_ULL 1000ULL
#define NSEC_ULL 1ULL

#define PSMI_TRUE   1
#define PSMI_FALSE  0

#define PSMI_CYCLES_TO_SECSF(cycles)			\
		((double) cycles_to_nanosecs(cycles) / 1.0e9)

#define PSMI_PAGESIZE       psm3_getpagesize()
#define PSMI_POWEROFTWO(P)  (((P)&((P)-1)) == 0)
#define PSMI_ALIGNDOWN(p, P) (((uintptr_t)(p))&~((uintptr_t)((P)-1)))
#define PSMI_ALIGNUP(p, P)   (PSMI_ALIGNDOWN((uintptr_t)(p)+((uintptr_t)((P)-1)), (P)))

#define PSMI_MAKE_DRIVER_VERSION(major, minor) ((major)<<16 | ((minor) & 0xffff))

#ifdef PSM_DEBUG

/* The intent of the following two macros is to emit an internal error if a size of a
   'member' is not as expected, violating an assumption in the code. There are some
   problems with the implementation of this code:

   The first macro creates a static const variable with ABSOLUTELY NO references
   to them.  For example there are ABSOLUTELY NO uses of the second macro in the
   PSM code. This is not completely pure. GCC version 5, for example, emits a
   warning for defining a static const when it is not referenced.

   A better implementation of the intent of this code is to use static_assert()
   so that at compile time the violations can be caught and corrected - not at
   run time.  */

#define PSMI_STRICT_SIZE_DECL(member, sz) static const size_t __psm3_ss_ ## member = sz
#define PSMI_STRICT_SIZE_VERIFY(member, sz)				\
	do {								\
		if (__psm3_ss_ ## member != (sz)) {			\
			char errmsg[64];				\
			snprintf(errmsg, 32, "Internal error: %s "	\
					"size doesn't match expected %d bytes",	\
					STRINGIFY(member), (int) __psm3_ss_ ## member);	\
			exit(-1);					\
		}							\
	} while (0)

#else

#define PSMI_STRICT_SIZE_DECL(member, sz)   /* nothing */
#define PSMI_STRICT_SIZE_VERIFY(member, sz) /* nothing */

#endif /*  PSM_DEBUG */

#endif /* _PSMI_HELP_H */
