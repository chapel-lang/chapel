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

#include "SobolQRNG/sobol_gpu.h"
#include "pi.h"
#include <starpu_cuda.h>

#define MAXNBLOCKS	128
#define MAXTHREADSPERBLOCK	256

static __global__ void monte_carlo(TYPE *random_numbers_x, TYPE *random_numbers_y,
						unsigned n, unsigned *output_cnt)
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
		TYPE x = random_numbers_x[ind];
		TYPE y = random_numbers_y[ind];
		TYPE dist = (x*x + y*y);

		unsigned success = (dist <= 1.0f)?1:0;

		scnt[threadIdx.x] += success;

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
		output_cnt[blockIdx.x] = scnt[0];

	__syncthreads();
}

static __global__ void sum_per_block_cnt(unsigned *output_cnt, unsigned *cnt)
{
	__shared__ unsigned accumulator[MAXNBLOCKS];

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
		*cnt = accumulator[0];
}

extern "C" void cuda_kernel(void *descr[], void *cl_arg)
{
	cudaError_t cures;

	unsigned *directions = (unsigned *)STARPU_VECTOR_GET_PTR(descr[0]);
	unsigned nx = NSHOT_PER_TASK;

	/* Generate Random numbers */
	float *random_numbers;
	cudaMalloc((void **)&random_numbers, 2*nx*sizeof(float));
	STARPU_ASSERT(random_numbers);
	
	sobolGPU(2*nx/n_dimensions, n_dimensions, directions, random_numbers);
	cudaStreamSynchronize(starpu_cuda_get_local_stream());

	TYPE *random_numbers_x = &random_numbers[0];
	TYPE *random_numbers_y = &random_numbers[nx];

	unsigned *cnt = (unsigned *)STARPU_VECTOR_GET_PTR(descr[1]);

	/* How many blocks do we use ? */ 
	unsigned nblocks = 128; // TODO

	STARPU_ASSERT(nblocks <= MAXNBLOCKS);
	
	unsigned *per_block_cnt;
	cudaMalloc((void **)&per_block_cnt, nblocks*sizeof(unsigned));

	STARPU_ASSERT((nx % nblocks) == 0);

	/* How many threads per block ? At most 256, but no more threads than
	 * there are entries to process per block. */
	unsigned nthread_per_block = STARPU_MIN(MAXTHREADSPERBLOCK, (nx / nblocks));

	/* each entry of per_block_cnt contains the number of successful shots
	 * in the corresponding block. */
	monte_carlo<<<nblocks, nthread_per_block, 0, starpu_cuda_get_local_stream()>>>(random_numbers_x, random_numbers_y, nx, per_block_cnt);

	/* Note that we do not synchronize between kernel calls because there is an implicit serialization */

	/* compute the total number of successful shots by adding the elements
	 * of the per_block_cnt array */
	sum_per_block_cnt<<<1, nblocks, 0, starpu_cuda_get_local_stream()>>>(per_block_cnt, cnt);
	cures = cudaStreamSynchronize(starpu_cuda_get_local_stream());
	if (cures)
		STARPU_CUDA_REPORT_ERROR(cures);

	cudaFree(per_block_cnt);
	cudaFree(random_numbers);
}
