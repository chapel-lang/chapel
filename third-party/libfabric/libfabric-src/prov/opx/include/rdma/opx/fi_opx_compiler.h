/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021 Cornelis Networks.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _FI_PROV_OPX_COMPILER_H_
#define _FI_PROV_OPX_COMPILER_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#define L2_CACHE_LINE_SIZE	(64)

#ifdef NDEBUG // No Debug, Optimizing
#define __OPX_INLINE__ static inline
#define __OPX_FORCE_INLINE__ static inline __attribute__ ((always_inline))
#define __OPX_FLATTEN__ static __attribute__ ((flatten))

/* https://github.com/ofiwg/libfabric/issues/7916
 * It's bad to use always-inline and flatten compiler attributes 
 * in the same function.  It also doesn't make much sense
 */
//#define __OPX_FORCE_INLINE_AND_FLATTEN__ static inline __attribute__ ((always_inline, flatten))
#else // NDEBUG
#define __OPX_INLINE__  static inline
#define __OPX_FORCE_INLINE__ static inline
#define __OPX_FLATTEN__ static
//#define __OPX_FORCE_INLINE_AND_FLATTEN__ static inline
#endif // NDEBUG

static inline void fi_opx_compiler_msync_writes()
{
#if defined(__riscv)

#if defined(__riscv_xlen) && (__riscv_xlen == 64)
 	asm volatile ("fence ow,ow" :  :  : "memory");
#else
#error "Unsupported CPU type"
#endif

#else
 	asm volatile ("sfence" :  :  : "memory");
#endif
}

static inline void fi_opx_compiler_msync_reads()
{
#if defined(__riscv)

#if defined(__riscv_xlen) && (__riscv_xlen == 64)
	asm volatile ("fence ir,ir" :  :  : "memory");
#else
#error "Unsupported CPU type"
#endif

#else
 	asm volatile ("lfence" :  :  : "memory");
#endif
}

#define fi_opx_compiler_barrier()		__asm__ __volatile__ ( "" ::: "memory" )


static inline
uint64_t fi_opx_compiler_load_u64(volatile uint64_t * const variable)
{
	return *variable;	/* aligned loads are atomic */
}

static inline
void fi_opx_compiler_store_u64(volatile uint64_t * const variable, const uint64_t value)
{
	*variable = value;	/* aligned stores are atomic */
	return;
}

static inline
void fi_opx_compiler_inc_u64(volatile uint64_t * const variable)
{
#if defined(__riscv)

#if defined(__riscv_xlen) && (__riscv_xlen == 64)
        (*variable) += 1;
#else
#error "Unsupported CPU type"
#endif

#else
	__asm__ __volatile__ ("lock ; incq %0"
				: "=m" (*variable)
				: "m" (*variable));
#endif
	return;
}

static inline
uint64_t fi_opx_compiler_fetch_and_inc_u64(volatile uint64_t * const variable)
{
	uint64_t value = 1;
#if defined(__riscv)

#if defined(__riscv_xlen) && (__riscv_xlen == 64)
	const uint64_t tmp = (*variable);
	(*variable) = value;
	value = tmp;
	(*variable) = (*variable) + value;
#else
#error "Unsupported CPU type"
#endif

#else
	__asm__ __volatile__ ("lock ; xadd %0,%1"
				: "=r" (value), "=m" (*variable)
				: "0" (value), "m" (*variable));
#endif
	return value;
}

static inline
void fi_opx_compiler_dec_u64(volatile uint64_t * const variable)
{
#if defined(__riscv)

#if defined(__riscv_xlen) && (__riscv_xlen == 64)
	(*variable) -= 1;
#else
#error "Unsupported CPU type"
#endif

#else
	__asm__ __volatile__ ("lock ; decq %0"
				: "=m" (*variable)
				: "m" (*variable));
#endif
	return;
}




static inline
void memcpy64(void *dst, const void *src) {

fprintf(stderr, "%s:%s():%d abort!\n", __FILE__, __func__, __LINE__); abort();
	assert(dst);
	assert(src);

	((double*)dst)[0] = ((const double*)src)[0];
	((double*)dst)[1] = ((const double*)src)[1];
	((double*)dst)[2] = ((const double*)src)[2];
	((double*)dst)[3] = ((const double*)src)[3];
	((double*)dst)[4] = ((const double*)src)[4];
	((double*)dst)[5] = ((const double*)src)[5];
	((double*)dst)[6] = ((const double*)src)[6];
	((double*)dst)[7] = ((const double*)src)[7];
}
static inline
void memcpy64_dual(void *dst1, void *dst2, const void *src) {

fprintf(stderr, "%s:%s():%d abort!\n", __FILE__, __func__, __LINE__); abort();
	assert(dst1);
	assert(dst2);
	assert(src);

	((double*)dst1)[0] = ((double*)dst2)[0] = ((const double*)src)[0];
	((double*)dst1)[1] = ((double*)dst2)[1] = ((const double*)src)[1];
	((double*)dst1)[2] = ((double*)dst2)[2] = ((const double*)src)[2];
	((double*)dst1)[3] = ((double*)dst2)[3] = ((const double*)src)[3];
	((double*)dst1)[4] = ((double*)dst2)[4] = ((const double*)src)[4];
	((double*)dst1)[5] = ((double*)dst2)[5] = ((const double*)src)[5];
	((double*)dst1)[6] = ((double*)dst2)[6] = ((const double*)src)[6];
	((double*)dst1)[7] = ((double*)dst2)[7] = ((const double*)src)[7];
}

#endif /* _FI_PROV_OPX_COMPILER_H_ */
