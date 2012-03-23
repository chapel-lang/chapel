/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
 * Copyright (C) 2010  Centre National de la Recherche Scientifique
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

#include <starpu.h>
#include <starpu_cuda.h>

#include "cg.h"

#define MAXNBLOCKS	128
#define MAXTHREADSPERBLOCK	256

static __global__ void dot_device(TYPE *vx, TYPE *vy, unsigned n, TYPE *dot_array)
{
	__shared__ TYPE scnt[MAXTHREADSPERBLOCK];

	/* Do we have a successful shot ? */
	const int tid = threadIdx.x + blockIdx.x*blockDim.x;

	const int nthreads = gridDim.x * blockDim.x;

	/* Blank the shared mem buffer */
	if (threadIdx.x < MAXTHREADSPERBLOCK)
		scnt[threadIdx.x] = (TYPE)0.0;

	__syncthreads();

	int ind;
	for (ind = tid; ind < n; ind += nthreads)
	{ 
		TYPE x = vx[ind];
		TYPE y = vy[ind];

		scnt[threadIdx.x] += (x*y);
	}

	__syncthreads();

	/* Perform a reduction to compute the sum on each thread within that block */

	/* NB: We assume that the number of threads per block is a power of 2 ! */
	unsigned s;
	for (s = blockDim.x/2; s!=0; s>>=1)
	{
		if (threadIdx.x < s)
			scnt[threadIdx.x] += scnt[threadIdx.x + s];

		__syncthreads();
	}

	/* report the number of successful shots in the block */
	if (threadIdx.x == 0)
		dot_array[blockIdx.x] = scnt[0];

	__syncthreads();
}

static __global__ void gather_dot_device(TYPE *dot_array, TYPE *dot)
{
	__shared__ TYPE accumulator[MAXNBLOCKS];

	unsigned i;

	/* Load the values from global mem */
	for (i = 0; i < blockDim.x; i++)
		accumulator[i] = dot_array[i];

	__syncthreads();

	/* Perform a reduction in shared memory */
	unsigned s;
	for (s = blockDim.x/2; s!=0; s>>=1)
	{
		if (threadIdx.x < s)
			accumulator[threadIdx.x] += accumulator[threadIdx.x + s];

		__syncthreads();
	}


	/* Save the result in global memory */
	if (threadIdx.x == 0)
		*dot = *dot + accumulator[0];
}

extern "C" void dot_host(TYPE *x, TYPE *y, unsigned nelems, TYPE *dot)
{
	/* How many blocks do we use ? */ 
	unsigned nblocks = 128; // TODO
	STARPU_ASSERT(nblocks <= MAXNBLOCKS);
	
	TYPE *per_block_sum;
	cudaMalloc((void **)&per_block_sum, nblocks*sizeof(TYPE));

	STARPU_ASSERT((nelems % nblocks) == 0);

	/* How many threads per block ? At most 256, but no more threads than
	 * there are entries to process per block. */
	unsigned nthread_per_block = STARPU_MIN(MAXTHREADSPERBLOCK, (nelems / nblocks));

	/* each entry of per_block_sum contains the number of successful shots
	 * in the corresponding block. */
	dot_device<<<nblocks, nthread_per_block, 0, starpu_cuda_get_local_stream()>>>(x, y, nelems, per_block_sum);

	/* Note that we do not synchronize between kernel calls because there
	 * is an implicit serialization */
	gather_dot_device<<<1, nblocks, 0, starpu_cuda_get_local_stream()>>>(per_block_sum, dot);

	cudaError_t cures;
	cures = cudaStreamSynchronize(starpu_cuda_get_local_stream());
	if (cures)
		STARPU_CUDA_REPORT_ERROR(cures);

	cudaFree(per_block_sum);
}

static __global__ void zero_vector_device(TYPE *x, unsigned nelems, unsigned nelems_per_thread)
{
	unsigned i;
	unsigned first_i = blockDim.x * blockIdx.x + threadIdx.x;

	for (i = first_i; i < nelems; i += nelems_per_thread)
		x[i] = 0.0;
}

extern "C" void zero_vector(TYPE *x, unsigned nelems)
{
	unsigned nblocks = STARPU_MIN(128, nelems);
	unsigned nthread_per_block = STARPU_MIN(MAXTHREADSPERBLOCK, (nelems / nblocks));

	unsigned nelems_per_thread = nelems / (nblocks * nthread_per_block);

	zero_vector_device<<<nblocks, nthread_per_block, 0, starpu_cuda_get_local_stream()>>>(x, nelems, nelems_per_thread);
}
