/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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

#include "xlu.h"
#include <math.h>

#define str(s) #s
#define xstr(s)        str(s)
#define STARPU_LU_STR(name)  xstr(STARPU_LU(name))

#ifdef STARPU_USE_CUDA
static const TYPE p1 =  1.0f;
static const TYPE m1 = -1.0f;
#endif

/*
 *   U22 
 */

static inline void STARPU_LU(common_u22)(void *descr[],
				int s, __attribute__((unused)) void *_args)
{
	TYPE *right 	= (TYPE *)STARPU_MATRIX_GET_PTR(descr[0]);
	TYPE *left 	= (TYPE *)STARPU_MATRIX_GET_PTR(descr[1]);
	TYPE *center 	= (TYPE *)STARPU_MATRIX_GET_PTR(descr[2]);

	unsigned dx = STARPU_MATRIX_GET_NX(descr[2]);
	unsigned dy = STARPU_MATRIX_GET_NY(descr[2]);
	unsigned dz = STARPU_MATRIX_GET_NY(descr[0]);

	unsigned ld12 = STARPU_MATRIX_GET_LD(descr[0]);
	unsigned ld21 = STARPU_MATRIX_GET_LD(descr[1]);
	unsigned ld22 = STARPU_MATRIX_GET_LD(descr[2]);

#ifdef STARPU_USE_CUDA
	cublasStatus status;
	cudaError_t cures;
#endif

	switch (s)
	{
		case 0:
			CPU_GEMM("N", "N", dy, dx, dz, 
				(TYPE)-1.0, right, ld21, left, ld12,
				(TYPE)1.0, center, ld22);
			break;

#ifdef STARPU_USE_CUDA
		case 1:
		{
			CUBLAS_GEMM('n', 'n', dx, dy, dz,
				*(CUBLAS_TYPE*)&m1, (CUBLAS_TYPE *)right, ld21, (CUBLAS_TYPE *)left, ld12,
				*(CUBLAS_TYPE*)&p1, (CUBLAS_TYPE *)center, ld22);

			status = cublasGetError();
			if (STARPU_UNLIKELY(status != CUBLAS_STATUS_SUCCESS))
				STARPU_ABORT();

			if (STARPU_UNLIKELY((cures = cudaThreadSynchronize()) != cudaSuccess))
				STARPU_CUDA_REPORT_ERROR(cures);

			break;
		}
#endif
		default:
			STARPU_ABORT();
			break;
	}
}

void STARPU_LU(cpu_u22)(void *descr[], void *_args)
{
	STARPU_LU(common_u22)(descr, 0, _args);
}

#ifdef STARPU_USE_CUDA
void STARPU_LU(cublas_u22)(void *descr[], void *_args)
{
	STARPU_LU(common_u22)(descr, 1, _args);
}
#endif /* STARPU_USE_CUDA */

static struct starpu_perfmodel STARPU_LU(model_22) =
{
	.type = STARPU_HISTORY_BASED,
#ifdef STARPU_ATLAS
	.symbol = STARPU_LU_STR(lu_model_22_atlas)
#elif defined(STARPU_GOTO)
	.symbol = STARPU_LU_STR(lu_model_22_goto)
#else
	.symbol = STARPU_LU_STR(lu_model_22)
#endif
};

struct starpu_codelet cl22 =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {STARPU_LU(cpu_u22), NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {STARPU_LU(cublas_u22), NULL},
#endif
	.nbuffers = 3,
	.modes = {STARPU_R, STARPU_R, STARPU_RW},
	.model = &STARPU_LU(model_22)
};

/*
 * U12
 */

static inline void STARPU_LU(common_u12)(void *descr[],
				int s, __attribute__((unused)) void *_args)
{
	TYPE *sub11;
	TYPE *sub12;

	sub11 = (TYPE *)STARPU_MATRIX_GET_PTR(descr[0]);	
	sub12 = (TYPE *)STARPU_MATRIX_GET_PTR(descr[1]);

	unsigned ld11 = STARPU_MATRIX_GET_LD(descr[0]);
	unsigned ld12 = STARPU_MATRIX_GET_LD(descr[1]);

	unsigned nx12 = STARPU_MATRIX_GET_NX(descr[1]);
	unsigned ny12 = STARPU_MATRIX_GET_NY(descr[1]);

#ifdef STARPU_USE_CUDA
	cublasStatus status;
	cudaError_t cures;
#endif

	/* solve L11 U12 = A12 (find U12) */
	switch (s)
	{
		case 0:
			CPU_TRSM("L", "L", "N", "N", nx12, ny12,
					(TYPE)1.0, sub11, ld11, sub12, ld12);
			break;
#ifdef STARPU_USE_CUDA
		case 1:
			CUBLAS_TRSM('L', 'L', 'N', 'N', ny12, nx12,
					*(CUBLAS_TYPE*)&p1, (CUBLAS_TYPE*)sub11, ld11, (CUBLAS_TYPE*)sub12, ld12);

			status = cublasGetError();
			if (STARPU_UNLIKELY(status != CUBLAS_STATUS_SUCCESS))
				STARPU_ABORT();

			if (STARPU_UNLIKELY((cures = cudaThreadSynchronize()) != cudaSuccess))
				STARPU_CUDA_REPORT_ERROR(cures);

			break;
#endif
		default:
			STARPU_ABORT();
			break;
	}
}

void STARPU_LU(cpu_u12)(void *descr[], void *_args)
{
	STARPU_LU(common_u12)(descr, 0, _args);
}

#ifdef STARPU_USE_CUDA
void STARPU_LU(cublas_u12)(void *descr[], void *_args)
{
	STARPU_LU(common_u12)(descr, 1, _args);
}
#endif /* STARPU_USE_CUDA */

static struct starpu_perfmodel STARPU_LU(model_12) =
{
	.type = STARPU_HISTORY_BASED,
#ifdef STARPU_ATLAS
	.symbol = STARPU_LU_STR(lu_model_12_atlas)
#elif defined(STARPU_GOTO)
	.symbol = STARPU_LU_STR(lu_model_12_goto)
#else
	.symbol = STARPU_LU_STR(lu_model_12)
#endif
};

struct starpu_codelet cl12 =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {STARPU_LU(cpu_u12), NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {STARPU_LU(cublas_u12), NULL},
#endif
	.nbuffers = 2,
	.modes = {STARPU_R, STARPU_RW},
	.model = &STARPU_LU(model_12)
};

/* 
 * U21
 */

static inline void STARPU_LU(common_u21)(void *descr[],
				int s, __attribute__((unused)) void *_args)
{
	TYPE *sub11;
	TYPE *sub21;

	sub11 = (TYPE *)STARPU_MATRIX_GET_PTR(descr[0]);
	sub21 = (TYPE *)STARPU_MATRIX_GET_PTR(descr[1]);

	unsigned ld11 = STARPU_MATRIX_GET_LD(descr[0]);
	unsigned ld21 = STARPU_MATRIX_GET_LD(descr[1]);

	unsigned nx21 = STARPU_MATRIX_GET_NX(descr[1]);
	unsigned ny21 = STARPU_MATRIX_GET_NY(descr[1]);
	
#ifdef STARPU_USE_CUDA
	cublasStatus status;
#endif

	switch (s)
	{
		case 0:
			CPU_TRSM("R", "U", "N", "U", nx21, ny21,
					(TYPE)1.0, sub11, ld11, sub21, ld21);
			break;
#ifdef STARPU_USE_CUDA
		case 1:
			CUBLAS_TRSM('R', 'U', 'N', 'U', ny21, nx21,
					*(CUBLAS_TYPE*)&p1, (CUBLAS_TYPE*)sub11, ld11, (CUBLAS_TYPE*)sub21, ld21);

			status = cublasGetError();
			if (status != CUBLAS_STATUS_SUCCESS)
				STARPU_ABORT();

			cudaThreadSynchronize();

			break;
#endif
		default:
			STARPU_ABORT();
			break;
	}
}

void STARPU_LU(cpu_u21)(void *descr[], void *_args)
{
	STARPU_LU(common_u21)(descr, 0, _args);
}

#ifdef STARPU_USE_CUDA
void STARPU_LU(cublas_u21)(void *descr[], void *_args)
{
	STARPU_LU(common_u21)(descr, 1, _args);
}
#endif 

static struct starpu_perfmodel STARPU_LU(model_21) =
{
	.type = STARPU_HISTORY_BASED,
#ifdef STARPU_ATLAS
	.symbol = STARPU_LU_STR(lu_model_21_atlas)
#elif defined(STARPU_GOTO)
	.symbol = STARPU_LU_STR(lu_model_21_goto)
#else
	.symbol = STARPU_LU_STR(lu_model_21)
#endif
};

struct starpu_codelet cl21 =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {STARPU_LU(cpu_u21), NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {STARPU_LU(cublas_u21), NULL},
#endif
	.nbuffers = 2,
	.modes = {STARPU_R, STARPU_RW},
	.model = &STARPU_LU(model_21)
};

/*
 *	U11
 */

static inline void STARPU_LU(common_u11)(void *descr[],
				int s, __attribute__((unused)) void *_args)
{
	TYPE *sub11;

	sub11 = (TYPE *)STARPU_MATRIX_GET_PTR(descr[0]); 

	unsigned long nx = STARPU_MATRIX_GET_NX(descr[0]);
	unsigned long ld = STARPU_MATRIX_GET_LD(descr[0]);

	unsigned long z;

	switch (s)
	{
		case 0:
			for (z = 0; z < nx; z++)
			{
				TYPE pivot;
				pivot = sub11[z+z*ld];
				STARPU_ASSERT(fpclassify(pivot) != FP_ZERO);
		
				CPU_SCAL(nx - z - 1, (1.0/pivot), &sub11[z+(z+1)*ld], ld);
		
				CPU_GER(nx - z - 1, nx - z - 1, -1.0,
						&sub11[(z+1)+z*ld], 1,
						&sub11[z+(z+1)*ld], ld,
						&sub11[(z+1) + (z+1)*ld],ld);
			}
			break;
#ifdef STARPU_USE_CUDA
		case 1:
			for (z = 0; z < nx; z++)
			{
				TYPE pivot;
				TYPE inv_pivot;
				cudaMemcpy(&pivot, &sub11[z+z*ld], sizeof(TYPE), cudaMemcpyDeviceToHost);
				cudaStreamSynchronize(0);

				STARPU_ASSERT(fpclassify(pivot) != FP_ZERO);
				
				inv_pivot = 1.0/pivot;
				CUBLAS_SCAL(nx - z - 1, *(CUBLAS_TYPE*)&inv_pivot, (CUBLAS_TYPE*)&sub11[z+(z+1)*ld], ld);
				
				CUBLAS_GER(nx - z - 1, nx - z - 1, *(CUBLAS_TYPE*)&m1,
						(CUBLAS_TYPE*)&sub11[(z+1)+z*ld], 1,
						(CUBLAS_TYPE*)&sub11[z+(z+1)*ld], ld,
						(CUBLAS_TYPE*)&sub11[(z+1) + (z+1)*ld],ld);
			}
			
			cudaThreadSynchronize();

			break;
#endif
		default:
			STARPU_ABORT();
			break;
	}
}

void STARPU_LU(cpu_u11)(void *descr[], void *_args)
{
	STARPU_LU(common_u11)(descr, 0, _args);
}

#ifdef STARPU_USE_CUDA
void STARPU_LU(cublas_u11)(void *descr[], void *_args)
{
	STARPU_LU(common_u11)(descr, 1, _args);
}
#endif /* STARPU_USE_CUDA */

static struct starpu_perfmodel STARPU_LU(model_11) =
{
	.type = STARPU_HISTORY_BASED,
#ifdef STARPU_ATLAS
	.symbol = STARPU_LU_STR(lu_model_11_atlas)
#elif defined(STARPU_GOTO)
	.symbol = STARPU_LU_STR(lu_model_11_goto)
#else
	.symbol = STARPU_LU_STR(lu_model_11)
#endif
};

struct starpu_codelet cl11 =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {STARPU_LU(cpu_u11), NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {STARPU_LU(cublas_u11), NULL},
#endif
	.nbuffers = 1,
	.modes = {STARPU_RW},
	.model = &STARPU_LU(model_11)
};

/*
 *	U11 with pivoting
 */

static inline void STARPU_LU(common_u11_pivot)(void *descr[],
				int s, void *_args)
{
	TYPE *sub11;

	sub11 = (TYPE *)STARPU_MATRIX_GET_PTR(descr[0]); 

	unsigned long nx = STARPU_MATRIX_GET_NX(descr[0]);
	unsigned long ld = STARPU_MATRIX_GET_LD(descr[0]);

	unsigned long z;

	struct piv_s *piv = _args;
	unsigned *ipiv = piv->piv;
	unsigned first = piv->first;

	switch (s)
	{
		case 0:
			for (z = 0; z < nx; z++)
			{
				TYPE pivot;
				pivot = sub11[z+z*ld];

				if (fabs((double)(pivot)) < PIVOT_THRESHHOLD)
				{

					/* find the pivot */
					int piv_ind = CPU_IAMAX(nx - z, &sub11[z*(ld+1)], ld);

					ipiv[z + first] = piv_ind + z + first;

					/* swap if needed */
					if (piv_ind != 0)
					{
						CPU_SWAP(nx, &sub11[z*ld], 1, &sub11[(z+piv_ind)*ld], 1);
					}

					pivot = sub11[z+z*ld];
				}
			
				STARPU_ASSERT(pivot != 0.0);

				CPU_SCAL(nx - z - 1, (1.0/pivot), &sub11[z+(z+1)*ld], ld);
		
				CPU_GER(nx - z - 1, nx - z - 1, -1.0,
						&sub11[(z+1)+z*ld], 1,
						&sub11[z+(z+1)*ld], ld,
						&sub11[(z+1) + (z+1)*ld],ld);
			}

			break;
#ifdef STARPU_USE_CUDA
		case 1:
			for (z = 0; z < nx; z++)
			{
				TYPE pivot;
				TYPE inv_pivot;
				cudaMemcpy(&pivot, &sub11[z+z*ld], sizeof(TYPE), cudaMemcpyDeviceToHost);
				cudaStreamSynchronize(0);

				if (fabs((double)(pivot)) < PIVOT_THRESHHOLD)
				{
					/* find the pivot */
					int piv_ind = CUBLAS_IAMAX(nx - z, (CUBLAS_TYPE*)&sub11[z*(ld+1)], ld) - 1;
	
					ipiv[z + first] = piv_ind + z + first;

					/* swap if needed */
					if (piv_ind != 0)
					{
						CUBLAS_SWAP(nx, (CUBLAS_TYPE*)&sub11[z*ld], 1, (CUBLAS_TYPE*)&sub11[(z+piv_ind)*ld], 1);
					}

					cudaMemcpy(&pivot, &sub11[z+z*ld], sizeof(TYPE), cudaMemcpyDeviceToHost);
					cudaStreamSynchronize(0);
				}

				STARPU_ASSERT(pivot != 0.0);
				
				inv_pivot = 1.0/pivot;
				CUBLAS_SCAL(nx - z - 1, *(CUBLAS_TYPE*)&inv_pivot, (CUBLAS_TYPE*)&sub11[z+(z+1)*ld], ld);
				
				CUBLAS_GER(nx - z - 1, nx - z - 1, *(CUBLAS_TYPE*)&m1,
						(CUBLAS_TYPE*)&sub11[(z+1)+z*ld], 1,
						(CUBLAS_TYPE*)&sub11[z+(z+1)*ld], ld,
						(CUBLAS_TYPE*)&sub11[(z+1) + (z+1)*ld],ld);
				
			}

			cudaThreadSynchronize();

			break;
#endif
		default:
			STARPU_ABORT();
			break;
	}
}

void STARPU_LU(cpu_u11_pivot)(void *descr[], void *_args)
{
	STARPU_LU(common_u11_pivot)(descr, 0, _args);
}

#ifdef STARPU_USE_CUDA
void STARPU_LU(cublas_u11_pivot)(void *descr[], void *_args)
{
	STARPU_LU(common_u11_pivot)(descr, 1, _args);
}
#endif /* STARPU_USE_CUDA */

static struct starpu_perfmodel STARPU_LU(model_11_pivot) =
{
	.type = STARPU_HISTORY_BASED,
#ifdef STARPU_ATLAS
	.symbol = STARPU_LU_STR(lu_model_11_pivot_atlas)
#elif defined(STARPU_GOTO)
	.symbol = STARPU_LU_STR(lu_model_11_pivot_goto)
#else
	.symbol = STARPU_LU_STR(lu_model_11_pivot)
#endif
};

struct starpu_codelet cl11_pivot =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {STARPU_LU(cpu_u11_pivot), NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {STARPU_LU(cublas_u11_pivot), NULL},
#endif
	.nbuffers = 1,
	.modes = {STARPU_RW},
	.model = &STARPU_LU(model_11_pivot)
};

/*
 *	Pivoting
 */

static inline void STARPU_LU(common_pivot)(void *descr[],
				int s, void *_args)
{
	TYPE *matrix;

	matrix = (TYPE *)STARPU_MATRIX_GET_PTR(descr[0]); 
	unsigned long nx = STARPU_MATRIX_GET_NX(descr[0]);
	unsigned long ld = STARPU_MATRIX_GET_LD(descr[0]);

	unsigned row;

	struct piv_s *piv = _args;
	unsigned *ipiv = piv->piv;
	unsigned first = piv->first;

	switch (s)
	{
		case 0:
			for (row = 0; row < nx; row++)
			{
				unsigned rowpiv = ipiv[row+first] - first;
				if (rowpiv != row)
				{
					CPU_SWAP(nx, &matrix[row*ld], 1, &matrix[rowpiv*ld], 1);
				}
			}
			break;
#ifdef STARPU_USE_CUDA
		case 1:
			for (row = 0; row < nx; row++)
			{
				unsigned rowpiv = ipiv[row+first] - first;
				if (rowpiv != row)
				{
					CUBLAS_SWAP(nx, (CUBLAS_TYPE*)&matrix[row*ld], 1, (CUBLAS_TYPE*)&matrix[rowpiv*ld], 1);
				}
			}

			cudaThreadSynchronize();

			break;
#endif
		default:
			STARPU_ABORT();
			break;
	}
}

void STARPU_LU(cpu_pivot)(void *descr[], void *_args)
{
	STARPU_LU(common_pivot)(descr, 0, _args);
}

#ifdef STARPU_USE_CUDA
void STARPU_LU(cublas_pivot)(void *descr[], void *_args)
{
	STARPU_LU(common_pivot)(descr, 1, _args);
}

#endif /* STARPU_USE_CUDA */

static struct starpu_perfmodel STARPU_LU(model_pivot) =
{
	.type = STARPU_HISTORY_BASED,
#ifdef STARPU_ATLAS
	.symbol = STARPU_LU_STR(lu_model_pivot_atlas)
#elif defined(STARPU_GOTO)
	.symbol = STARPU_LU_STR(lu_model_pivot_goto)
#else
	.symbol = STARPU_LU_STR(lu_model_pivot)
#endif
};

struct starpu_codelet cl_pivot =
{
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {STARPU_LU(cpu_pivot), NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {STARPU_LU(cublas_pivot), NULL},
#endif
	.nbuffers = 1,
	.modes = {STARPU_RW},
	.model = &STARPU_LU(model_pivot)
};
