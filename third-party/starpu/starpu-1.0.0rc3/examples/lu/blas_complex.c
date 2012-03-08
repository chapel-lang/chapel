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

#include <ctype.h>
#include <stdio.h>
#include <complex.h>

#include <starpu.h>
#include "blas_complex.h"

/*
    This files contains BLAS wrappers for the different BLAS implementations
  (eg. REFBLAS, STARPU_ATLAS, GOTOBLAS ...). We assume a Fortran orientation as most
  libraries do not supply C-based ordering.
 */

#ifdef STARPU_ATLAS
#error not implemented
#elif defined(STARPU_GOTO) || defined(STARPU_SYSTEM_BLAS)
#error not implemented
#elif defined(STARPU_MKL)

inline void CGEMM(char *transa, char *transb, int M, int N, int K, 
			complex float alpha, complex float *A, int lda, complex float *B, int ldb, 
			complex float beta, complex float *C, int ldc)
{
	cgemm_(transa, transb, &M, &N, &K, &alpha,
			 A, &lda, B, &ldb,
			 &beta, C, &ldc);	
}

inline void ZGEMM(char *transa, char *transb, int M, int N, int K, 
			complex double alpha, complex double *A, int lda, complex double *B, int ldb, 
			complex double beta, complex double *C, int ldc)
{
	zgemm_(transa, transb, &M, &N, &K, &alpha,
			 A, &lda, B, &ldb,
			 &beta, C, &ldc);	
}


inline void CGEMV(char *transa, int M, int N, complex float alpha, complex float *A, int lda,
		complex float *X, int incX, complex float beta, complex float *Y, int incY)
{
	cgemv_(transa, &M, &N, &alpha, A, &lda, X, &incX, &beta, Y, &incY);
}

inline void ZGEMV(char *transa, int M, int N, complex double alpha, complex double *A, int lda,
		complex double *X, int incX, complex double beta, complex double *Y, int incY)
{
	zgemv_(transa, &M, &N, &alpha, A, &lda, X, &incX, &beta, Y, &incY);
}

inline float SCASUM(int N, complex float *X, int incX)
{
	return scasum_(&N, X, &incX);
}

inline double DZASUM(int N, complex double *X, int incX)
{
	return dzasum_(&N, X, &incX);
}

void CSCAL(int N, complex float alpha, complex float *X, int incX)
{
	cscal_(&N, &alpha, X, &incX);
}

void ZSCAL(int N, complex double alpha, complex double *X, int incX)
{
	zscal_(&N, &alpha, X, &incX);
}

void CTRSM (const char *side, const char *uplo, const char *transa,
                   const char *diag, const int m, const int n,
                   const complex float alpha, const complex float *A, const int lda,
                   complex float *B, const int ldb)
{
	ctrsm_(side, uplo, transa, diag, &m, &n, &alpha, A, &lda, B, &ldb);
}

void ZTRSM (const char *side, const char *uplo, const char *transa,
                   const char *diag, const int m, const int n,
                   const complex double alpha, const complex double *A, const int lda,
                   complex double *B, const int ldb)
{
	ztrsm_(side, uplo, transa, diag, &m, &n, &alpha, A, &lda, B, &ldb);
}

void CSYR (const char *uplo, const int n, const complex float alpha,
                  const complex float *x, const int incx, complex float *A, const int lda)
{
	csyr_(uplo, &n, &alpha, x, &incx, A, &lda); 
}

void CSYRK (const char *uplo, const char *trans, const int n,
                   const int k, const complex float alpha, const complex float *A,
                   const int lda, const complex float beta, complex float *C,
                   const int ldc)
{
	csyrk_(uplo, trans, &n, &k, &alpha, A, &lda, &beta, C, &ldc); 
}

void CGERU(const int m, const int n, const complex float alpha,
                  const complex float *x, const int incx, const complex float *y,
                  const int incy, complex float *A, const int lda)
{
	cgeru_(&m, &n, &alpha, x, &incx, y, &incy, A, &lda);
}

void ZGERU(const int m, const int n, const complex double alpha,
                  const complex double *x, const int incx, const complex double *y,
                  const int incy, complex double *A, const int lda)
{
	zgeru_(&m, &n, &alpha, x, &incx, y, &incy, A, &lda);
}

void CTRSV (const char *uplo, const char *trans, const char *diag, 
                   const int n, const complex float *A, const int lda, complex float *x, 
                   const int incx)
{
	ctrsv_(uplo, trans, diag, &n, A, &lda, x, &incx);
}

void CTRMM(const char *side, const char *uplo, const char *transA,
                 const char *diag, const int m, const int n,
                 const complex float alpha, const complex float *A, const int lda,
                 complex float *B, const int ldb)
{
	ctrmm_(side, uplo, transA, diag, &m, &n, &alpha, A, &lda, B, &ldb);
}

void ZTRMM(const char *side, const char *uplo, const char *transA,
                 const char *diag, const int m, const int n,
                 const complex double alpha, const complex double *A, const int lda,
                 complex double *B, const int ldb)
{
	ztrmm_(side, uplo, transA, diag, &m, &n, &alpha, A, &lda, B, &ldb);
}

void CTRMV(const char *uplo, const char *transA, const char *diag,
                 const int n, const complex float *A, const int lda, complex float *X,
                 const int incX)
{
	ctrmv_(uplo, transA, diag, &n, A, &lda, X, &incX);
}

void CAXPY(const int n, const complex float alpha, complex float *X, const int incX, complex float *Y, const int incY)
{
	caxpy_(&n, &alpha, X, &incX, Y, &incY);
}

void ZAXPY(const int n, const complex double alpha, complex double *X, const int incX, complex double *Y, const int incY)
{
	zaxpy_(&n, &alpha, X, &incX, Y, &incY);
}

int ICAMAX (const int n, complex float *X, const int incX)
{
    int retVal;
    retVal = icamax_ (&n, X, &incX);
    return retVal;
}

int IZAMAX (const int n, complex double *X, const int incX)
{
    int retVal;
    retVal = izamax_ (&n, X, &incX);
    return retVal;
}

complex float CDOTU(const int n, const complex float *x, const int incx, const complex float *y, const int incy)
{
	complex float retVal = 0;

	/* GOTOBLAS will return a FLOATRET which is a double, not a float */
	retVal = (float)cdotu_(&n, x, &incx, y, &incy);

	return retVal;
}

complex double ZDOTU(const int n, const complex double *x, const int incx, const complex double *y, const int incy)
{
	return zdotu_(&n, x, &incx, y, &incy);
}

void CSWAP(const int n, complex float *X, const int incX, complex float *Y, const int incY)
{
	cswap_(&n, X, &incX, Y, &incY);
}

void ZSWAP(const int n, complex double *X, const int incX, complex double *Y, const int incY)
{
	zswap_(&n, X, &incX, Y, &incY);
}


#else
#error "no BLAS lib available..."
#endif
