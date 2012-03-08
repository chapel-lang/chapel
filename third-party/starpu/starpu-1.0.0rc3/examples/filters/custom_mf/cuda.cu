/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012 inria
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
#include "custom_types.h"
#include "custom_interface.h"

static __global__ void scal_cuda(unsigned n,
				 float *x,
				 float *y)
{
        unsigned i =  blockIdx.x*blockDim.x + threadIdx.x;

	if (i < n)
		x[i] *= y[i];
}

extern "C" void custom_scal_cuda_func(void *buffers[], void *_args)
{
	(void) _args;

	unsigned int n = CUSTOM_GET_NX(buffers[0]);
	float *x = (float*) CUSTOM_GET_X_PTR(buffers[0]);
	float *y = (float*) CUSTOM_GET_Y_PTR(buffers[0]);

	unsigned threads_per_block = 64;
	unsigned nblocks = (n + threads_per_block-1) / threads_per_block;
        scal_cuda<<<nblocks,threads_per_block,2,starpu_cuda_get_local_stream()>>>(n, x, y);

	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
