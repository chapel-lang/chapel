/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Institut National de Recherche en Informatique et Automatique
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
#include "multiformat_types.h"
#include "../test_interfaces.h"
#include "../../../helper.h"

extern struct test_config multiformat_config;

static __global__ void multiformat_cuda(struct struct_of_arrays *soa, unsigned n,
					int *err, int factor)
{
        unsigned i =  blockIdx.x*blockDim.x + threadIdx.x;

	if (i >= n)
		return;

	if (soa->x[i] != i * factor || soa->y[i] != i * factor)
	{
		*err = 1;
	}
	else
	{
		soa->x[i] = -soa->x[i];
		soa->y[i] = -soa->y[i];
	}
}

extern "C" void test_multiformat_cuda_func(void *buffers[], void *args)
{
	FPRINTF(stderr, "Entering %s\n", __func__);
	int factor;
	int *ret;
	cudaError_t error;
	unsigned int n = STARPU_MULTIFORMAT_GET_NX(buffers[0]);
	struct struct_of_arrays *soa;

	soa = (struct struct_of_arrays *) STARPU_MULTIFORMAT_GET_CUDA_PTR(buffers[0]);
	unsigned threads_per_block = 64;
	unsigned nblocks = (n + threads_per_block-1) / threads_per_block;
	factor = *(int *) args;

	error = cudaMalloc(&ret, sizeof(int));
	if (error != cudaSuccess)
		STARPU_CUDA_REPORT_ERROR(error);

	error = cudaMemcpy(ret,
			   &multiformat_config.copy_failed,
			   sizeof(int),
			   cudaMemcpyHostToDevice);
	if (error != cudaSuccess)
		STARPU_CUDA_REPORT_ERROR(error);

        multiformat_cuda<<<nblocks,threads_per_block,2,starpu_cuda_get_local_stream()>>>(soa, n, ret, factor);

	error = cudaMemcpy(&multiformat_config.copy_failed,
			   ret,
			   sizeof(int),
			   cudaMemcpyDeviceToHost);
	
	cudaFree(ret);
	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
