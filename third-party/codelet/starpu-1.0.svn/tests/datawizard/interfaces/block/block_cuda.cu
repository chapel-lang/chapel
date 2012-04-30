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

extern struct test_config block_config;

static __global__ void block_cuda(int *block,
				  int nx, int ny, int nz,
				  unsigned ldy, unsigned ldz,
				  float factor, int *err)
{
        int i, j, k;
	int val = 0;

        for (k = 0; k < nz ;k++)
	{
                for (j = 0; j < ny ;j++)
		{
                        for(i = 0; i < nx ;i++)
			{
				if (block[(k*ldz)+(j*ldy)+i] != factor * val)
				{
					*err = 1;
					return;
				}
				else
				{
					block[(k*ldz)+(j*ldy)+i] *= -1;
					val++;
				}
			}
                }
        }
}

extern "C" void test_block_cuda_func(void *buffers[], void *args)
{
	cudaError_t error;
	int *ret;

	error = cudaMalloc(&ret, sizeof(int));
	if (error != cudaSuccess)
		STARPU_CUDA_REPORT_ERROR(error);

	error = cudaMemcpy(ret, &block_config.copy_failed, sizeof(int), cudaMemcpyHostToDevice);
	if (error != cudaSuccess)
		STARPU_CUDA_REPORT_ERROR(error);

	int nx = STARPU_BLOCK_GET_NX(buffers[0]);
	int ny = STARPU_BLOCK_GET_NY(buffers[0]);
	int nz = STARPU_BLOCK_GET_NZ(buffers[0]);
        unsigned ldy = STARPU_BLOCK_GET_LDY(buffers[0]);
        unsigned ldz = STARPU_BLOCK_GET_LDZ(buffers[0]);
	int *block = (int *) STARPU_BLOCK_GET_PTR(buffers[0]);
	int factor = *(int*) args;

        block_cuda<<<1,1, 0, starpu_cuda_get_local_stream()>>>
		(block, nx, ny, nz, ldy, ldz, factor, ret);
	error = cudaMemcpy(&block_config.copy_failed, ret, sizeof(int), cudaMemcpyDeviceToHost);
	if (error != cudaSuccess)
		STARPU_CUDA_REPORT_ERROR(error);

	cudaFree(ret);
	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
