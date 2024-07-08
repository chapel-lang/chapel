/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

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

  Copyright(c) 2015 Intel Corporation.

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

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#ifndef UTILS_MACROS_H
#define UTILS_MACROS_H

#include <sys/uio.h>
#include <sys/types.h>
#include <stdint.h>

#ifdef __inline__
#undef __inline__
#endif
#define __inline__ inline __attribute__((always_inline, unused))

#include "sysdep.h"
#include "bit_ops.h"

/* these aren't implemented for user mode, which is OK until we multi-thread */
typedef struct _atomic {
	uint32_t counter;
} atomic_t;			/* no atomic_t type in user-land */
#define atomic_set(a, v) ((a)->counter = (v))
#define atomic_inc_return(a)  (++(a)->counter)

#if defined(__GNUC__)
#ifndef likely
#define likely(x)    __builtin_expect(!!(x), 1L)
#endif
#ifndef unlikely
#define unlikely(x)  __builtin_expect(!!(x), 0L)
#endif
#ifndef if_pt
#define if_pt(cond) if (likely(cond))
#endif
#ifndef if_pf
#define if_pf(cond) if (unlikely(cond))
#endif
#define _Pragma_unlikely
#define _Pragma_likely
#else
#error "Unsupported compiler"
#endif

#define yield() sched_yield()
#endif /* UTILS_MACROS_H */
