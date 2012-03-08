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
#define TYPE complex float
#define CUBLAS_TYPE cuComplex

#define STARPU_LU(name)       starpu_clu_##name
#define COMPLEX_LU

#ifdef STARPU_HAVE_MAGMA
#include <magmablas.h>
#define CUBLAS_GEMM	magmablas_cgemm
#define CUBLAS_TRSM	magmablas_ctrsm
#else
#define CUBLAS_GEMM	cublasCgemm
#define CUBLAS_TRSM	cublasCtrsm
#endif

#define CUBLAS_SCAL	cublasCscal
#define CUBLAS_GER	cublasCgeru
#define CUBLAS_SWAP	cublasCswap
#define CUBLAS_IAMAX	cublasIcamax

#define CPU_GEMM	CGEMM
#define CPU_TRSM	CTRSM
#define CPU_SCAL	CSCAL
#define CPU_GER		CGERU
#define CPU_SWAP	CSWAP

#define CPU_TRMM	CTRMM
#define CPU_AXPY	CAXPY
#define CPU_ASUM	SCASUM
#define CPU_IAMAX	ICAMAX

#define PIVOT_THRESHHOLD	10e-5
