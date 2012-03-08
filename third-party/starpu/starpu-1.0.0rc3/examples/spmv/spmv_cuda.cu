/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010  Université de Bordeaux 1
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

#include <starpu.h>
#include <starpu_cuda.h>

#define MIN(a,b)	((a)<(b)?(a):(b))

extern "C" __global__ 
void spmv_kernel(uint32_t nnz, uint32_t nrow, float *nzval, uint32_t *colind, uint32_t *rowptr, 
			uint32_t firstentry, uint32_t elemsize, 
			float *vecin, uint32_t nx_in, uint32_t elemsize1, float * vecout, uint32_t nx_out, uint32_t elemsize2)
{
	/* only one dimension is used here */
	unsigned nthreads = gridDim.x*blockDim.x;
	unsigned threadid = threadIdx.x + blockIdx.x*blockDim.x;

	unsigned rowstart = threadid * ((nrow + (nthreads - 1))/nthreads);
	unsigned rowend = MIN(nrow, (threadid+1) * ((nrow + (nthreads - 1))/nthreads));

	unsigned row;
	for (row = rowstart; row < rowend; row++)
	{
		float tmp = 0.0f;
		unsigned index;

		unsigned firstindex = rowptr[row] - firstentry;
		unsigned lastindex = rowptr[row+1] - firstentry; 

		for (index = firstindex; index < lastindex; index++)
		{
			tmp += nzval[index]*vecin[colind[index]];
		}

		vecout[row] = tmp;
	}
}

extern "C" __global__ 
void spmv_kernel_3(uint32_t nnz, uint32_t nrow, float *nzval, uint32_t *colind, uint32_t *rowptr, 
			uint32_t firstentry, 
			float *vecin, uint32_t nx_in, float * vecout, uint32_t nx_out)
{
	/* only one dimension is used here */
	unsigned block_rowstart = blockIdx.x*( (nrow + gridDim.x - 1)/gridDim.x );
	unsigned block_rowend = MIN((blockIdx.x+1)*( (nrow + gridDim.x - 1)/gridDim.x ), nrow);

	unsigned row;
	for (row = block_rowstart + threadIdx.x; row < block_rowend; row+=blockDim.x)
	{
		float tmp = 0.0f;
		unsigned index;

		unsigned firstindex = rowptr[row] - firstentry;
		unsigned lastindex = rowptr[row+1] - firstentry;

		for (index = firstindex; index < lastindex; index++)
		{
			tmp += nzval[index]*vecin[colind[index]];
		}

		vecout[row] = tmp;
	}
	

}

extern "C" void spmv_kernel_cuda(void *descr[], void *args)
{
	uint32_t nnz = STARPU_CSR_GET_NNZ(descr[0]);
	uint32_t nrow = STARPU_CSR_GET_NROW(descr[0]);
	float *nzval = (float *)STARPU_CSR_GET_NZVAL(descr[0]);
	uint32_t *colind = STARPU_CSR_GET_COLIND(descr[0]);
	uint32_t *rowptr = STARPU_CSR_GET_ROWPTR(descr[0]);
	uint32_t firstentry = STARPU_CSR_GET_FIRSTENTRY(descr[0]);

	float *vecin = (float *)STARPU_VECTOR_GET_PTR(descr[1]);
	uint32_t nx_in = STARPU_VECTOR_GET_NX(descr[1]);

	float *vecout = (float *)STARPU_VECTOR_GET_PTR(descr[2]);
	uint32_t nx_out = STARPU_VECTOR_GET_NX(descr[2]);

	dim3 dimBlock(8, 1);
	dim3 dimGrid(512, 1);

	spmv_kernel_3<<<dimGrid, dimBlock, 0, starpu_cuda_get_local_stream()>>>
		(nnz, nrow, nzval, colind, rowptr, firstentry, vecin, nx_in, vecout, nx_out);

	cudaStreamSynchronize(starpu_cuda_get_local_stream());

}


