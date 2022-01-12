/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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
#ifndef _FI_PROV_BGQ_COMPILER_H_
#define _FI_PROV_BGQ_COMPILER_H_


#if (defined(__xlc__) || defined(__xlC__)) && !defined(__OPTIMIZE__)
#undef VECTOR_LOAD_NU
#undef VECTOR_STORE_NU
#define VECTOR_LOAD_NU(si,sb,f0) \
  do { \
    asm volatile("qvlfdx %0,%1,%2" : "=v" (f0) : "b" (si), "r" (sb)); \
  } while(0)
#define VECTOR_STORE_NU(si,sb,f0) \
  do { \
    asm volatile("qvstfdx %0,%1,%2" :: "v" (f0), "b" (si), "r" (sb) :"memory"); \
  } while(0)
#endif

#if defined(__xlc__) || defined(__xlC__)
#ifdef __OPTIMIZE__
#define DECLARE_FP_REGISTER(n) register double f##n asm("f" #n)
#else
#define DECLARE_FP_REGISTER(n) register vector4double f##n asm("v" #n)
#endif /* __OPTIMIZE__ */
#else /* GNU */
#define DECLARE_FP_REGISTER(n) register double f##n asm("fr" #n)
#endif


static inline
void qpx_memcpy64(void *dst, const void *src) {

	assert(dst);
	assert(src);
	assert(((uintptr_t)dst&0x1Full) == 0);
	assert(((uintptr_t)src&0x1Full) == 0);

#if defined(__VECTOR4DOUBLE__)
	const vector4double v0 = vec_ld(0, (double*)src);
	const vector4double v1 = vec_ld(32, (double*)src);
	vec_st(v0, 0, (double*)dst);
	vec_st(v1, 32, (double*)dst);
#elif defined(__OPTIMIZE__)
#  if (defined(__xlc__) || defined(__xlC__))
#    error "xlc but not vector4double"
#  else
	DECLARE_FP_REGISTER(0);
	DECLARE_FP_REGISTER(1);
	VECTOR_LOAD_NU((void*)src, 0, f0);
	VECTOR_LOAD_NU((void*)src, 32, f1);
	VECTOR_STORE_NU(dst, 0, f0);
	VECTOR_STORE_NU(dst, 32, f1);
#  endif
#else
	((double*)dst)[0] = ((const double*)src)[0];
	((double*)dst)[1] = ((const double*)src)[1];
	((double*)dst)[2] = ((const double*)src)[2];
	((double*)dst)[3] = ((const double*)src)[3];
	((double*)dst)[4] = ((const double*)src)[4];
	((double*)dst)[5] = ((const double*)src)[5];
	((double*)dst)[6] = ((const double*)src)[6];
	((double*)dst)[7] = ((const double*)src)[7];
#endif
}

#endif /* _FI_PROV_BGQ_COMPILER_H_ */
