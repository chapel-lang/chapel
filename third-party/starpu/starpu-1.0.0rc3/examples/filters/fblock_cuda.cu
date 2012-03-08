/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
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

#include <starpu.h>
#include <starpu_cuda.h>

static __global__ void fblock_cuda(int *block, int nx, int ny, int nz, unsigned ldy, unsigned ldz, float factor)
{
        int i, j, k;
        for(k=0; k<nz ; k++)
	{
                for(j=0; j<ny ; j++)
		{
                        for(i=0; i<nx ; i++)
                                block[(k*ldz)+(j*ldy)+i] = factor;
                }
        }
}

extern "C" void cuda_func(void *buffers[], void *_args)
{
        int *factor = (int *)_args;
	int *block = (int *)STARPU_BLOCK_GET_PTR(buffers[0]);
	int nx = (int)STARPU_BLOCK_GET_NX(buffers[0]);
	int ny = (int)STARPU_BLOCK_GET_NY(buffers[0]);
	int nz = (int)STARPU_BLOCK_GET_NZ(buffers[0]);
        unsigned ldy = STARPU_BLOCK_GET_LDY(buffers[0]);
        unsigned ldz = STARPU_BLOCK_GET_LDZ(buffers[0]);

        /* TODO: use more blocks and threads in blocks */
        fblock_cuda<<<1,1, 0, starpu_cuda_get_local_stream()>>>(block, nx, ny, nz, ldy, ldz, *factor);

	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}
