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
#include "../test_interfaces.h"

extern struct test_config bcsr_config;

__global__ void bcsr_cuda(int *nzval, uint32_t nnz, int *err, int factor)
{
        unsigned i =  blockIdx.x*blockDim.x + threadIdx.x;

	if (i >= nnz)
		return;

	if (nzval[i] != i*factor)
		*err = 1;
	else
		nzval[i] = -nzval[i];
}

extern "C" void test_bcsr_cuda_func(void *buffers[], void *args)
{
	int factor;
	int *ret;
	int *val;
	cudaError_t error;
	uint32_t nnz = STARPU_BCSR_GET_NNZ(buffers[0]);
	unsigned threads_per_block = 64;
	unsigned nblocks = (nnz + threads_per_block-1) / threads_per_block;

	factor = *(int *) args;
	//val = (int *) starpu_bcsr_get_local_nzval((starpu_data_handle_t)buffers[0]);
	val = (int *) STARPU_BCSR_GET_NZVAL(buffers[0]);

	error = cudaMalloc(&ret, sizeof(int));
	if (error != cudaSuccess)
		STARPU_CUDA_REPORT_ERROR(error);

	error = cudaMemcpy(ret,
			   &bcsr_config.copy_failed,
			   sizeof(int),
			   cudaMemcpyHostToDevice);
	if (error != cudaSuccess)
		STARPU_CUDA_REPORT_ERROR(error);

        bcsr_cuda<<<nblocks,threads_per_block,2,starpu_cuda_get_local_stream()>>>
		(val, nnz, ret, factor);

	error = cudaMemcpy(&bcsr_config.copy_failed,
			   ret,
			   sizeof(int),
			   cudaMemcpyDeviceToHost);
	
	cudaFree(ret);
	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
