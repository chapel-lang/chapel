/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
 * Copyright (C) 2010-2011  Université de Bordeaux 1
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

static __global__ void vector_mult_cuda(unsigned *val, unsigned n)
{
        unsigned i =  blockIdx.x*blockDim.x + threadIdx.x;

	if (i < n)
               val[i] *= 2;
}

extern "C" void scal_func_cuda(void *buffers[], void *_args)
{
        unsigned n = STARPU_VECTOR_GET_NX(buffers[0]);
        unsigned *val = (unsigned *)STARPU_VECTOR_GET_PTR(buffers[0]);
	unsigned threads_per_block = 64;
	unsigned nblocks = (n + threads_per_block-1) / threads_per_block;

        vector_mult_cuda<<<nblocks,threads_per_block,0,starpu_cuda_get_local_stream()>>>(val, n);

	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
