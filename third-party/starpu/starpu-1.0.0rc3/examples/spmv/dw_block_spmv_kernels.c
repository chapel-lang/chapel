/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
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

#include "dw_block_spmv.h"

/*
 *   U22 
 */

static inline void common_block_spmv(void *descr[], int s, __attribute__((unused)) void *_args)
{
	/* printf("22\n"); */
	float *block 	= (float *)STARPU_MATRIX_GET_PTR(descr[0]);
	float *in 	= (float *)STARPU_VECTOR_GET_PTR(descr[1]);
	float *out 	= (float *)STARPU_VECTOR_GET_PTR(descr[2]);

	unsigned dx = STARPU_MATRIX_GET_NX(descr[0]);
	unsigned dy = STARPU_MATRIX_GET_NY(descr[0]);

	unsigned ld = STARPU_MATRIX_GET_LD(descr[0]);

	switch (s)
	{
		case 0:
			cblas_sgemv(CblasRowMajor, CblasNoTrans, dx, dy, 1.0f, block, ld, in, 1, 1.0f, out, 1);
			break;
#ifdef STARPU_USE_CUDA
		case 1:
			cublasSgemv ('t', dx, dy, 1.0f, block, ld, in, 1, 1.0f, out, 1);
			break;
#endif
		default:
			STARPU_ABORT();
			break;
	}
}

void cpu_block_spmv(void *descr[], void *_args)
{
/*	printf("CPU CODELET \n"); */

	common_block_spmv(descr, 0, _args);
}

#ifdef STARPU_USE_CUDA
void cublas_block_spmv(void *descr[], void *_args)
{
/*	printf("CUBLAS CODELET \n"); */

	common_block_spmv(descr, 1, _args);
}
#endif /* STARPU_USE_CUDA */
