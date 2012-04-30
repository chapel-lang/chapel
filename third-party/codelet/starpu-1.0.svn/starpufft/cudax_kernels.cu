/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010  Université de Bordeaux 1
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

#define _externC extern "C"
#include "cudax_kernels.h"

/* Note: these assume that the sizes are powers of two */

#define VARS_1d \
	unsigned start = threadIdx.x + blockIdx.x * blockDim.x; \
	unsigned numthreads = blockDim.x * gridDim.x;

#define DISTRIB_1d(n, func,args) \
	unsigned threads_per_block = 128; \
\
	if (n < threads_per_block) \
	{			   \
		dim3 dimGrid(n); \
		func <<<dimGrid, 1, 0, starpu_cuda_get_local_stream()>>> args; \
	} 					\
	else 					\
	{				     \
		dim3 dimGrid(n / threads_per_block); \
		dim3 dimBlock(threads_per_block); \
		func <<<dimGrid, dimBlock, 0, starpu_cuda_get_local_stream()>>> args; \
	} \
	cudaStreamSynchronize(starpu_cuda_get_local_stream()); \

extern "C" __global__ void
STARPUFFT(cuda_twist1_1d)(const _cuComplex *in, _cuComplex *twisted1, unsigned i, unsigned n1, unsigned n2)
{
	unsigned j;
	VARS_1d
	unsigned end = n2;

	for (j = start; j < end; j += numthreads)
		twisted1[j] = in[i+j*n1];
}

extern "C" void
STARPUFFT(cuda_twist1_1d_host)(const _cuComplex *in, _cuComplex *twisted1, unsigned i, unsigned n1, unsigned n2)
{
	DISTRIB_1d(n2, STARPUFFT(cuda_twist1_1d), (in, twisted1, i, n1, n2));
}

extern "C" __global__ void
STARPUFFT(cuda_twiddle_1d)(_cuComplex * out, const _cuComplex * roots, unsigned n, unsigned i)
{
	unsigned j;
	VARS_1d
	unsigned end = n;

	for (j = start; j < end; j += numthreads)
		out[j] = _cuCmul(out[j], roots[i*j]);
	return;
}

extern "C" void
STARPUFFT(cuda_twiddle_1d_host)(_cuComplex *out, const _cuComplex *roots, unsigned n, unsigned i)
{
	DISTRIB_1d(n, STARPUFFT(cuda_twiddle_1d), (out, roots, n, i));
}

#define VARS_2d \
	unsigned startx = threadIdx.x + blockIdx.x * blockDim.x; \
	unsigned starty = threadIdx.y + blockIdx.y * blockDim.y; \
	unsigned numthreadsx = blockDim.x * gridDim.x; \
	unsigned numthreadsy = blockDim.y * gridDim.y;

/* FIXME: introduce threads_per_dim_n / m instead */
#define DISTRIB_2d(n, m, func, args) \
	unsigned threads_per_dim = 16; \
	if (n < threads_per_dim) \
	{				   \
		if (m < threads_per_dim) \
		{			    \
			dim3 dimGrid(n, m); \
			func <<<dimGrid, 1, 0, starpu_cuda_get_local_stream()>>> args; \
		} \
		else \
		{					      \
			dim3 dimGrid(1, m / threads_per_dim); \
			dim3 dimBlock(n, threads_per_dim); \
			func <<<dimGrid, dimBlock, 0, starpu_cuda_get_local_stream()>>> args; \
		} \
	} \
	else \
	{				   \
		if (m < threads_per_dim) \
		{					      \
			dim3 dimGrid(n / threads_per_dim, 1); \
			dim3 dimBlock(threads_per_dim, m); \
			func <<<dimGrid, dimBlock, 0, starpu_cuda_get_local_stream()>>> args; \
		} \
		else \
		{							\
			dim3 dimGrid(n / threads_per_dim, m / threads_per_dim); \
			dim3 dimBlock(threads_per_dim, threads_per_dim); \
			func <<<dimGrid, dimBlock, 0, starpu_cuda_get_local_stream()>>> args; \
		} \
	} \
	cudaStreamSynchronize(starpu_cuda_get_local_stream()); \

extern "C" __global__ void
STARPUFFT(cuda_twist1_2d)(const _cuComplex *in, _cuComplex *twisted1, unsigned i, unsigned j, unsigned n1, unsigned n2, unsigned m1, unsigned m2)
{
	unsigned k, l;
	VARS_2d
	unsigned endx = n2;
	unsigned endy = m2;
	unsigned m = m1*m2;

	for (k = startx; k < endx; k += numthreadsx)
		for (l = starty; l < endy; l += numthreadsy)
			twisted1[k*m2+l] = in[i*m+j+k*m*n1+l*m1];
}

extern "C" void
STARPUFFT(cuda_twist1_2d_host)(const _cuComplex *in, _cuComplex *twisted1, unsigned i, unsigned j, unsigned n1, unsigned n2, unsigned m1, unsigned m2)
{
	DISTRIB_2d(n2, m2, STARPUFFT(cuda_twist1_2d), (in, twisted1, i, j, n1, n2, m1, m2));
}

extern "C" __global__ void
STARPUFFT(cuda_twiddle_2d)(_cuComplex * out, const _cuComplex * roots0, const _cuComplex * roots1, unsigned n2, unsigned m2, unsigned i, unsigned j)
{
	unsigned k, l;
	VARS_2d
	unsigned endx = n2;
	unsigned endy = m2;

	for (k = startx; k < endx ; k += numthreadsx)
		for (l = starty; l < endy ; l += numthreadsy)
			out[k*m2 + l] = _cuCmul(_cuCmul(out[k*m2 + l], roots0[i*k]), roots1[j*l]);
	return;
}

extern "C" void
STARPUFFT(cuda_twiddle_2d_host)(_cuComplex *out, const _cuComplex *roots0, const _cuComplex *roots1, unsigned n2, unsigned m2, unsigned i, unsigned j)
{
	DISTRIB_2d(n2, m2, STARPUFFT(cuda_twiddle_2d), (out, roots0, roots1, n2, m2, i, j));
}
