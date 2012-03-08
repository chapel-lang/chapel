/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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

#define MAXNBLOCKS	128
#define MAXTHREADSPERBLOCK	256

static __global__ void monte_carlo(float *x, float *y, unsigned n, unsigned long *output_cnt)
{
	__shared__ unsigned scnt[MAXTHREADSPERBLOCK];

	/* Do we have a successful shot ? */
	const int tid = threadIdx.x + blockIdx.x*blockDim.x;

	const int nthreads = gridDim.x * blockDim.x;

	/* Blank the shared mem buffer */
	if (threadIdx.x < MAXTHREADSPERBLOCK)
		scnt[threadIdx.x] = 0;

	__syncthreads();
	int ind;
	for (ind = tid; ind < n; ind += nthreads)
	{ 
		float xval = (2.0f * x[ind] - 1.0f);
		float yval = (2.0f * y[ind] - 1.0f);
		float dist = (xval*xval + yval*yval);

		unsigned long success = (dist <= 1.0f)?1:0;

		scnt[threadIdx.x] += success;

	}

	__syncthreads();

	/* Perform a reduction to compute the sum on each thread within that block */

	/* NB: We assume that the number of threads per block is a power of 2 ! */
	unsigned long s;
	for (s = blockDim.x/2; s!=0; s>>=1)
	{
		if (threadIdx.x < s)
			scnt[threadIdx.x] += scnt[threadIdx.x + s];

		__syncthreads();
	}

	/* report the number of successful shots in the block */
	if (threadIdx.x == 0)
		output_cnt[blockIdx.x] = scnt[0];

	__syncthreads();
}

static __global__ void sum_per_block_cnt(unsigned long *output_cnt, unsigned long *cnt)
{
	__shared__ unsigned long accumulator[MAXNBLOCKS];

	unsigned i;

	/* Load the values from global mem */
	for (i = 0; i < blockDim.x; i++)
		accumulator[i] = output_cnt[i];

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
		*cnt = *cnt + accumulator[0];
}

extern "C" void pi_redux_cuda_kernel(float *x, float *y, unsigned n, unsigned long *shot_cnt)
{
	cudaError_t cures;

	/* How many blocks do we use ? */ 
	unsigned nblocks = 128; // TODO
	STARPU_ASSERT(nblocks <= MAXNBLOCKS);
	STARPU_ASSERT((n % nblocks) == 0);
	
	unsigned long *per_block_cnt;
	cudaMalloc((void **)&per_block_cnt, nblocks*sizeof(unsigned long));

	/* How many threads per block ? At most 256, but no more threads than
	 * there are entries to process per block. */
	unsigned nthread_per_block = STARPU_MIN(MAXTHREADSPERBLOCK, (n / nblocks));

	/* each entry of per_block_cnt contains the number of successful shots
	 * in the corresponding block. */
	monte_carlo<<<nblocks, nthread_per_block, 0, starpu_cuda_get_local_stream()>>>(x, y, n, per_block_cnt);

	/* Note that we do not synchronize between kernel calls because there is an implicit serialization */

	/* compute the total number of successful shots by adding the elements
	 * of the per_block_cnt array */
	sum_per_block_cnt<<<1, nblocks, 0, starpu_cuda_get_local_stream()>>>(per_block_cnt, shot_cnt);
	cures = cudaStreamSynchronize(starpu_cuda_get_local_stream());
	if (cures)
		STARPU_CUDA_REPORT_ERROR(cures);

	cudaFree(per_block_cnt);
}
