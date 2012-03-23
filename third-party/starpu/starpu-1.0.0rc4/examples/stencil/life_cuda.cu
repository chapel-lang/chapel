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

#define _externC extern "C"
#include "stencil.h"
#include <starpu_cuda.h>

/* Heart of the stencil computation: compute a new state from an old one. */

extern "C" __global__ void
cuda_life_update(int bz, const TYPE *old, TYPE *newp, int nx, int ny, int nz, int ldy, int ldz, int iter)
{
	unsigned idx = threadIdx.x + blockIdx.x * blockDim.x;
	unsigned idy = threadIdx.y + blockIdx.y * blockDim.y;
	//unsigned idz = threadIdx.z + blockIdx.z * blockDim.z;
	unsigned idz = 0;
	unsigned stepx = blockDim.x * gridDim.x;
	unsigned stepy = blockDim.y * gridDim.y;
	//unsigned stepz = blockDim.z * gridDim.z;
	unsigned stepz = 1;
	unsigned x, y, z;
	unsigned num, alive;

	for (z = iter + idz; z < nz - iter; z += stepz)
		for (y = K + idy; y < ny - K; y += stepy)
		{
			for (x = K + idx; x < nx - K; x += stepx)
			{
				unsigned index = x + y*ldy + z*ldz;
				num = 0
                                        + old[index+1*ldy+0*ldz]
                                        + old[index+1*ldy+1*ldz]
                                        + old[index+0*ldy+1*ldz]
                                        + old[index-1*ldy+1*ldz]
                                        + old[index-1*ldy+0*ldz]
                                        + old[index-1*ldy-1*ldz]
                                        + old[index+0*ldy-1*ldz]
                                        + old[index+1*ldy-1*ldz]
					;
				alive = old[index];
				alive = (alive && num == 2) || num == 3;
				newp[index] = alive;
			}
		}
}

extern "C" void
cuda_life_update_host(int bz, const TYPE *old, TYPE *newp, int nx, int ny, int nz, int ldy, int ldz, int iter)
{
	unsigned max_parallelism = 512;
	unsigned threads_per_dim_x = max_parallelism;
	while (threads_per_dim_x / 2 >= nx)
		threads_per_dim_x /= 2;
	unsigned threads_per_dim_y = max_parallelism / threads_per_dim_x;
	while (threads_per_dim_y / 2 >= ny)
		threads_per_dim_y /= 2;
#if 0
	unsigned threads_per_dim_z = 4;
	dim3 dimBlock(threads_per_dim_x, threads_per_dim_y, threads_per_dim_z);
	dim3 dimGrid(nx / threads_per_dim_x, ny / threads_per_dim_y, nz / threads_per_dim_z);
#else
	dim3 dimBlock(threads_per_dim_x, threads_per_dim_y);
	dim3 dimGrid((nx + threads_per_dim_x-1) / threads_per_dim_x, (ny + threads_per_dim_y-1) / threads_per_dim_y);
#endif
	cuda_life_update <<<dimGrid, dimBlock, 0, starpu_cuda_get_local_stream()>>> (bz, old, newp, nx, ny, nz, ldy, ldz, iter);
}
