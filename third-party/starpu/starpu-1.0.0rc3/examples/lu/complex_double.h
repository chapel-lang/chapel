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


#include <complex.h>
#include "blas_complex.h"
#define TYPE complex double
#define CUBLAS_TYPE cuDoubleComplex

#define STARPU_LU(name)       starpu_zlu_##name
#define COMPLEX_LU

#ifdef STARPU_HAVE_MAGMA
#include <magmablas.h>
#define CUBLAS_GEMM	magmablas_zgemm
#define CUBLAS_TRSM	magmablas_ztrsm
#else
#define CUBLAS_GEMM	cublasZgemm
#define CUBLAS_TRSM	cublasZtrsm
#endif

#define CUBLAS_SCAL	cublasZscal
#define CUBLAS_GER	cublasZgeru
#define CUBLAS_SWAP	cublasZswap
#define CUBLAS_IAMAX	cublasIzamax

#define CPU_GEMM	ZGEMM
#define CPU_TRSM	ZTRSM
#define CPU_SCAL	ZSCAL
#define CPU_GER		ZGERU
#define CPU_SWAP	ZSWAP

#define CPU_TRMM	ZTRMM
#define CPU_AXPY	ZAXPY
#define CPU_ASUM	DZASUM
#define CPU_IAMAX	IZAMAX

#define PIVOT_THRESHHOLD	10e-5
