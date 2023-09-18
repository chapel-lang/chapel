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

#ifndef _HFI_i386_SYSDEP_H
#define _HFI_i386_SYSDEP_H

typedef struct cpuid {
        unsigned eax, ebx, ecx, edx;
} cpuid_t;

static __inline__ void
get_cpuid(const unsigned func, const unsigned subfunc, cpuid_t *id)
{
	unsigned a, b, c, d;

	asm (" \
	mov %4, %%eax \n\
	mov %5, %%ecx \n\
	cpuid \n\
	mov %%eax, %0 \n\
	mov %%ebx, %1 \n\
	mov %%ecx, %2 \n\
	mov %%edx, %3 \n\
	" : "=g" (a), "=g" (b), "=g" (c), "=g" (d)
	: "g" (func), "g" (subfunc)
	: "%eax", "%ebx", "%ecx", "%edx"
	);

	id->eax = a;
	id->ebx = b;
	id->ecx = c;
	id->edx = d;
}

static __inline__ uint64_t get_cycles(void)
{
	uint64_t v;
	uint32_t a, d;

	asm volatile ("rdtsc" : "=a" (a), "=d"(d));
	v = ((uint64_t) a) | (((uint64_t) d) << 32);

	return v;
}

#ifndef LOCK_PREFIX
#define LOCK_PREFIX "lock "
#endif

static __inline__ void ips_barrier()
{
	asm volatile ("" :  :  : "memory");
}

static __inline__ void ips_mb()
{
	asm volatile ("mfence" :  :  : "memory");
}

/* gcc-3.4 has a bug with this function body at -O0 */
static
#if defined(__GNUC__) && __GNUC__ == 3 && __GNUC_MINOR__ == 4
#else
__inline__
#endif
void ips_rmb()
{
	asm volatile ("" :  :  : "memory");
}

static __inline__ void ips_wmb()
{
	asm volatile ("sfence" :  :  : "memory");
}

static __inline__ void ips_sync_writes()
{
	asm volatile ("sfence" :  :  : "memory");
}

static __inline__ void ips_sync_reads()
{
	asm volatile ("lfence" :  :  : "memory");
}

static __inline__ uint32_t ips_cmpxchg(volatile uint32_t *ptr,
				       uint32_t old_val, uint32_t new_val)
{
	uint32_t prev;
	volatile uint32_t *__ptr = (volatile uint32_t *)(ptr);

	asm volatile(LOCK_PREFIX "cmpxchgl %2,%1"
		: "=a" (prev), "+m" (*__ptr)
		: "r" (new_val), "0" (old_val)
		: "memory");

	return prev;
}

typedef struct {
	volatile int32_t counter;
} ips_atomic_t;

#define ips_atomic_set(v, i)		  (((v)->counter) = (i))
#define ips_atomic_cmpxchg(p, oval, nval)	  \
	    ips_cmpxchg((volatile uint32_t *) &((p)->counter), oval, nval)

#if 0
static __inline__ int32_t
ips_cmpxchg(volatile int32_t *p, int32_t old_value, int32_t new_value)
{
	asm volatile ("lock cmpxchg %2, %0" :
		      "+m" (*p), "+a"(old_value) : "r"(new_value) : "memory");
	return old_value;
}
#endif

#endif /* _HFI_i386_SYSDEP_H */
