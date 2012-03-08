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

#define TYPE double
#define CUBLAS_TYPE TYPE

#define STARPU_LU(name)       starpu_dlu_##name

#ifdef STARPU_HAVE_MAGMA
#include <magmablas.h>
#define CUBLAS_GEMM	magmablas_dgemm
#define CUBLAS_TRSM	magmablas_dtrsm
#else
#define CUBLAS_GEMM	cublasDgemm
#define CUBLAS_TRSM	cublasDtrsm
#endif
#define CUBLAS_SCAL	cublasDscal
#define CUBLAS_GER	cublasDger
#define CUBLAS_SWAP	cublasDswap
#define CUBLAS_IAMAX	cublasIdamax

#define CPU_GEMM	DGEMM
#define CPU_TRSM	DTRSM
#define CPU_SCAL	DSCAL
#define CPU_GER		DGER
#define CPU_SWAP	DSWAP

#define CPU_TRMM	DTRMM
#define CPU_AXPY	DAXPY
#define CPU_ASUM	DASUM
#define CPU_IAMAX	IDAMAX

#define PIVOT_THRESHHOLD	10e-10
