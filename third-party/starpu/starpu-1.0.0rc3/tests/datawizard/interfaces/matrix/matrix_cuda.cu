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

extern struct test_config matrix_config;

__global__ void matrix_cuda(int *val, unsigned n, int *err, int factor)
{
        unsigned i =  blockIdx.x*blockDim.x + threadIdx.x;

	if (i >= n)
		return;

	if (val[i] != i*factor)
		*err = 1;
	else
		val[i] = -val[i];
}

extern "C" void test_matrix_cuda_func(void *buffers[], void *args)
{
	int factor;
	int *ret;
	int *val;
	cudaError_t error;
	unsigned int nx, ny, n;

	nx = STARPU_MATRIX_GET_NX(buffers[0]);
	ny = STARPU_MATRIX_GET_NY(buffers[0]);
	n = nx * ny;
	unsigned threads_per_block = 64;
	unsigned nblocks = (n + threads_per_block-1) / threads_per_block;
	factor = *(int *) args;
	val = (int *) STARPU_MATRIX_GET_PTR(buffers[0]);

	error = cudaMalloc(&ret, sizeof(int));
	if (error != cudaSuccess)
		STARPU_CUDA_REPORT_ERROR(error);

	error = cudaMemcpy(ret,
			   &matrix_config.copy_failed,
			   sizeof(int),
			   cudaMemcpyHostToDevice);
	if (error != cudaSuccess)
		STARPU_CUDA_REPORT_ERROR(error);

        matrix_cuda<<<nblocks,threads_per_block,2,starpu_cuda_get_local_stream()>>>(val, n, ret, factor);

	error = cudaMemcpy(&matrix_config.copy_failed,
			   ret,
			   sizeof(int),
			   cudaMemcpyDeviceToHost);
	
	cudaFree(ret);
	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
