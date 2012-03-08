/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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

#define TYPE float
#define MPI_TYPE	MPI_FLOAT

#define STARPU_PLU(name)       starpu_pslu_##name

#define CUBLAS_GEMM	cublasSgemm
#define CUBLAS_TRSM	cublasStrsm
#define CUBLAS_SCAL	cublasSscal
#define CUBLAS_GER	cublasSger
#define CUBLAS_SWAP	cublasSswap
#define CUBLAS_IAMAX	cublasIsamax

#define CPU_GEMM	SGEMM
#define CPU_GEMV	SGEMV
#define CPU_TRSM	STRSM
#define CPU_SCAL	SSCAL
#define CPU_GER		SGER
#define CPU_SWAP	SSWAP

#define CPU_TRMM	STRMM
#define CPU_AXPY	SAXPY
#define CPU_ASUM	SASUM
#define CPU_IAMAX	ISAMAX

#define PIVOT_THRESHHOLD	10e-5
