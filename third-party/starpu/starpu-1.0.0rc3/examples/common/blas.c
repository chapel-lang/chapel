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

#include <starpu.h>
#include "blas.h"

/*
    This files contains BLAS wrappers for the different BLAS implementations
  (eg. REFBLAS, STARPU_ATLAS, GOTOBLAS ...). We assume a Fortran orientation as most
  libraries do not supply C-based ordering.
 */

#ifdef STARPU_ATLAS

inline void SGEMM(char *transa, char *transb, int M, int N, int K, 
			float alpha, float *A, int lda, float *B, int ldb, 
			float beta, float *C, int ldc)
{
	enum CBLAS_TRANSPOSE ta = (toupper(transa[0]) == 'N')?CblasNoTrans:CblasTrans;
	enum CBLAS_TRANSPOSE tb = (toupper(transb[0]) == 'N')?CblasNoTrans:CblasTrans;

	cblas_sgemm(CblasColMajor, ta, tb,
			M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);				
}

inline void DGEMM(char *transa, char *transb, int M, int N, int K, 
			double alpha, double *A, int lda, double *B, int ldb, 
			double beta, double *C, int ldc)
{
	enum CBLAS_TRANSPOSE ta = (toupper(transa[0]) == 'N')?CblasNoTrans:CblasTrans;
	enum CBLAS_TRANSPOSE tb = (toupper(transb[0]) == 'N')?CblasNoTrans:CblasTrans;

	cblas_dgemm(CblasColMajor, ta, tb,
			M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);				
}

inline void SGEMV(char *transa, int M, int N, float alpha, float *A, int lda, float *X, int incX, float beta, float *Y, int incY)
{
	enum CBLAS_TRANSPOSE ta = (toupper(transa[0]) == 'N')?CblasNoTrans:CblasTrans;

	cblas_sgemv(CblasColMajor, ta, M, N, alpha, A, lda,
					X, incX, beta, Y, incY);
}

inline void DGEMV(char *transa, int M, int N, double alpha, double *A, int lda, double *X, int incX, double beta, double *Y, int incY)
{
	enum CBLAS_TRANSPOSE ta = (toupper(transa[0]) == 'N')?CblasNoTrans:CblasTrans;

	cblas_dgemv(CblasColMajor, ta, M, N, alpha, A, lda,
					X, incX, beta, Y, incY);
}

inline float SASUM(int N, float *X, int incX)
{
	return cblas_sasum(N, X, incX);
}

inline double DASUM(int N, double *X, int incX)
{
	return cblas_dasum(N, X, incX);
}

void SSCAL(int N, float alpha, float *X, int incX)
{
	cblas_sscal(N, alpha, X, incX);
}

void DSCAL(int N, double alpha, double *X, int incX)
{
	cblas_dscal(N, alpha, X, incX);
}

void STRSM (const char *side, const char *uplo, const char *transa,
                   const char *diag, const int m, const int n,
                   const float alpha, const float *A, const int lda,
                   float *B, const int ldb)
{
	enum CBLAS_SIDE side_ = (toupper(side[0]) == 'L')?CblasLeft:CblasRight;
	enum CBLAS_UPLO uplo_ = (toupper(uplo[0]) == 'U')?CblasUpper:CblasLower;
	enum CBLAS_TRANSPOSE transa_ = (toupper(transa[0]) == 'N')?CblasNoTrans:CblasTrans;
	enum CBLAS_DIAG diag_ = (toupper(diag[0]) == 'N')?CblasNonUnit:CblasUnit;

	cblas_strsm(CblasColMajor, side_, uplo_, transa_, diag_, m, n, alpha, A, lda, B, ldb);
}

void DTRSM (const char *side, const char *uplo, const char *transa,
                   const char *diag, const int m, const int n,
                   const double alpha, const double *A, const int lda,
                   double *B, const int ldb)
{
	enum CBLAS_SIDE side_ = (toupper(side[0]) == 'L')?CblasLeft:CblasRight;
	enum CBLAS_UPLO uplo_ = (toupper(uplo[0]) == 'U')?CblasUpper:CblasLower;
	enum CBLAS_TRANSPOSE transa_ = (toupper(transa[0]) == 'N')?CblasNoTrans:CblasTrans;
	enum CBLAS_DIAG diag_ = (toupper(diag[0]) == 'N')?CblasNonUnit:CblasUnit;

	cblas_dtrsm(CblasColMajor, side_, uplo_, transa_, diag_, m, n, alpha, A, lda, B, ldb);
}

void SSYR (const char *uplo, const int n, const float alpha,
                  const float *x, const int incx, float *A, const int lda)
{
	enum CBLAS_UPLO uplo_ = (toupper(uplo[0]) == 'U')?CblasUpper:CblasLower;

	cblas_ssyr(CblasColMajor, uplo_, n, alpha, x, incx, A, lda); 
}

void SSYRK (const char *uplo, const char *trans, const int n,
                   const int k, const float alpha, const float *A,
                   const int lda, const float beta, float *C,
                   const int ldc)
{
	enum CBLAS_UPLO uplo_ = (toupper(uplo[0]) == 'U')?CblasUpper:CblasLower;
	enum CBLAS_TRANSPOSE trans_ = (toupper(trans[0]) == 'N')?CblasNoTrans:CblasTrans;
	
	cblas_ssyrk(CblasColMajor, uplo_, trans_, n, k, alpha, A, lda, beta, C, ldc); 
}

void SGER(const int m, const int n, const float alpha,
                  const float *x, const int incx, const float *y,
                  const int incy, float *A, const int lda)
{
	cblas_sger(CblasColMajor, m, n, alpha, x, incx, y, incy, A, lda);
}

void DGER(const int m, const int n, const double alpha,
                  const double *x, const int incx, const double *y,
                  const int incy, double *A, const int lda)
{
	cblas_dger(CblasColMajor, m, n, alpha, x, incx, y, incy, A, lda);
}

void STRSV (const char *uplo, const char *trans, const char *diag, 
                   const int n, const float *A, const int lda, float *x, 
                   const int incx)
{
	enum CBLAS_UPLO uplo_ = (toupper(uplo[0]) == 'U')?CblasUpper:CblasLower;
	enum CBLAS_TRANSPOSE trans_ = (toupper(trans[0]) == 'N')?CblasNoTrans:CblasTrans;
	enum CBLAS_DIAG diag_ = (toupper(diag[0]) == 'N')?CblasNonUnit:CblasUnit;

	cblas_strsv(CblasColMajor, uplo_, trans_, diag_, n, A, lda, x, incx);
}

void STRMM(const char *side, const char *uplo, const char *transA,
                 const char *diag, const int m, const int n,
                 const float alpha, const float *A, const int lda,
                 float *B, const int ldb)
{
	enum CBLAS_SIDE side_ = (toupper(side[0]) == 'L')?CblasLeft:CblasRight;
	enum CBLAS_UPLO uplo_ = (toupper(uplo[0]) == 'U')?CblasUpper:CblasLower;
	enum CBLAS_TRANSPOSE transA_ = (toupper(transA[0]) == 'N')?CblasNoTrans:CblasTrans;
	enum CBLAS_DIAG diag_ = (toupper(diag[0]) == 'N')?CblasNonUnit:CblasUnit;

	cblas_strmm(CblasColMajor, side_, uplo_, transA_, diag_, m, n, alpha, A, lda, B, ldb);
}

void DTRMM(const char *side, const char *uplo, const char *transA,
                 const char *diag, const int m, const int n,
                 const double alpha, const double *A, const int lda,
                 double *B, const int ldb)
{
	enum CBLAS_SIDE side_ = (toupper(side[0]) == 'L')?CblasLeft:CblasRight;
	enum CBLAS_UPLO uplo_ = (toupper(uplo[0]) == 'U')?CblasUpper:CblasLower;
	enum CBLAS_TRANSPOSE transA_ = (toupper(transA[0]) == 'N')?CblasNoTrans:CblasTrans;
	enum CBLAS_DIAG diag_ = (toupper(diag[0]) == 'N')?CblasNonUnit:CblasUnit;

	cblas_dtrmm(CblasColMajor, side_, uplo_, transA_, diag_, m, n, alpha, A, lda, B, ldb);
}

void STRMV(const char *uplo, const char *transA, const char *diag,
                 const int n, const float *A, const int lda, float *X,
                 const int incX)
{
	enum CBLAS_UPLO uplo_ = (toupper(uplo[0]) == 'U')?CblasUpper:CblasLower;
	enum CBLAS_TRANSPOSE transA_ = (toupper(transA[0]) == 'N')?CblasNoTrans:CblasTrans;
	enum CBLAS_DIAG diag_ = (toupper(diag[0]) == 'N')?CblasNonUnit:CblasUnit;

	cblas_strmv(CblasColMajor, uplo_, transA_, diag_, n, A, lda, X, incX);
}

void SAXPY(const int n, const float alpha, float *X, const int incX, float *Y, const int incY)
{
	cblas_saxpy(n, alpha, X, incX, Y, incY);
}

void DAXPY(const int n, const double alpha, double *X, const int incX, double *Y, const int incY)
{
	cblas_daxpy(n, alpha, X, incX, Y, incY);
}

int ISAMAX (const int n, float *X, const int incX)
{
    int retVal;
    retVal = cblas_isamax(n, X, incX);
    return retVal;
}

int IDAMAX (const int n, double *X, const int incX)
{
    int retVal;
    retVal = cblas_idamax(n, X, incX);
    return retVal;
}

float SDOT(const int n, const float *x, const int incx, const float *y, const int incy)
{
	return cblas_sdot(n, x, incx, y, incy);
}

double DDOT(const int n, const double *x, const int incx, const double *y, const int incy)
{
	return cblas_ddot(n, x, incx, y, incy);
}

void SSWAP(const int n, float *x, const int incx, float *y, const int incy)
{
	cblas_sswap(n, x, incx, y, incy);
}

void DSWAP(const int n, double *x, const int incx, double *y, const int incy)
{
	cblas_dswap(n, x, incx, y, incy);
}

#elif defined(STARPU_GOTO) || defined(STARPU_SYSTEM_BLAS) || defined(STARPU_MKL)

inline void SGEMM(char *transa, char *transb, int M, int N, int K, 
			float alpha, float *A, int lda, float *B, int ldb, 
			float beta, float *C, int ldc)
{
	sgemm_(transa, transb, &M, &N, &K, &alpha,
			 A, &lda, B, &ldb,
			 &beta, C, &ldc);	
}

inline void DGEMM(char *transa, char *transb, int M, int N, int K, 
			double alpha, double *A, int lda, double *B, int ldb, 
			double beta, double *C, int ldc)
{
	dgemm_(transa, transb, &M, &N, &K, &alpha,
			 A, &lda, B, &ldb,
			 &beta, C, &ldc);	
}


inline void SGEMV(char *transa, int M, int N, float alpha, float *A, int lda,
		float *X, int incX, float beta, float *Y, int incY)
{
	sgemv_(transa, &M, &N, &alpha, A, &lda, X, &incX, &beta, Y, &incY);
}

inline void DGEMV(char *transa, int M, int N, double alpha, double *A, int lda,
		double *X, int incX, double beta, double *Y, int incY)
{
	dgemv_(transa, &M, &N, &alpha, A, &lda, X, &incX, &beta, Y, &incY);
}

inline float SASUM(int N, float *X, int incX)
{
	return sasum_(&N, X, &incX);
}

inline double DASUM(int N, double *X, int incX)
{
	return dasum_(&N, X, &incX);
}

void SSCAL(int N, float alpha, float *X, int incX)
{
	sscal_(&N, &alpha, X, &incX);
}

void DSCAL(int N, double alpha, double *X, int incX)
{
	dscal_(&N, &alpha, X, &incX);
}

void STRSM (const char *side, const char *uplo, const char *transa,
                   const char *diag, const int m, const int n,
                   const float alpha, const float *A, const int lda,
                   float *B, const int ldb)
{
	strsm_(side, uplo, transa, diag, &m, &n, &alpha, A, &lda, B, &ldb);
}

void DTRSM (const char *side, const char *uplo, const char *transa,
                   const char *diag, const int m, const int n,
                   const double alpha, const double *A, const int lda,
                   double *B, const int ldb)
{
	dtrsm_(side, uplo, transa, diag, &m, &n, &alpha, A, &lda, B, &ldb);
}

void SSYR (const char *uplo, const int n, const float alpha,
                  const float *x, const int incx, float *A, const int lda)
{
	ssyr_(uplo, &n, &alpha, x, &incx, A, &lda); 
}

void SSYRK (const char *uplo, const char *trans, const int n,
                   const int k, const float alpha, const float *A,
                   const int lda, const float beta, float *C,
                   const int ldc)
{
	ssyrk_(uplo, trans, &n, &k, &alpha, A, &lda, &beta, C, &ldc); 
}

void SGER(const int m, const int n, const float alpha,
                  const float *x, const int incx, const float *y,
                  const int incy, float *A, const int lda)
{
	sger_(&m, &n, &alpha, x, &incx, y, &incy, A, &lda);
}

void DGER(const int m, const int n, const double alpha,
                  const double *x, const int incx, const double *y,
                  const int incy, double *A, const int lda)
{
	dger_(&m, &n, &alpha, x, &incx, y, &incy, A, &lda);
}

void STRSV (const char *uplo, const char *trans, const char *diag, 
                   const int n, const float *A, const int lda, float *x, 
                   const int incx)
{
	strsv_(uplo, trans, diag, &n, A, &lda, x, &incx);
}

void STRMM(const char *side, const char *uplo, const char *transA,
                 const char *diag, const int m, const int n,
                 const float alpha, const float *A, const int lda,
                 float *B, const int ldb)
{
	strmm_(side, uplo, transA, diag, &m, &n, &alpha, A, &lda, B, &ldb);
}

void DTRMM(const char *side, const char *uplo, const char *transA,
                 const char *diag, const int m, const int n,
                 const double alpha, const double *A, const int lda,
                 double *B, const int ldb)
{
	dtrmm_(side, uplo, transA, diag, &m, &n, &alpha, A, &lda, B, &ldb);
}

void STRMV(const char *uplo, const char *transA, const char *diag,
                 const int n, const float *A, const int lda, float *X,
                 const int incX)
{
	strmv_(uplo, transA, diag, &n, A, &lda, X, &incX);
}

void SAXPY(const int n, const float alpha, float *X, const int incX, float *Y, const int incY)
{
	saxpy_(&n, &alpha, X, &incX, Y, &incY);
}

void DAXPY(const int n, const double alpha, double *X, const int incX, double *Y, const int incY)
{
	daxpy_(&n, &alpha, X, &incX, Y, &incY);
}

int ISAMAX (const int n, float *X, const int incX)
{
    int retVal;
    retVal = isamax_ (&n, X, &incX);
    return retVal;
}

int IDAMAX (const int n, double *X, const int incX)
{
    int retVal;
    retVal = idamax_ (&n, X, &incX);
    return retVal;
}

float SDOT(const int n, const float *x, const int incx, const float *y, const int incy)
{
	float retVal = 0;

	/* GOTOBLAS will return a FLOATRET which is a double, not a float */
	retVal = (float)sdot_(&n, x, &incx, y, &incy);

	return retVal;
}

double DDOT(const int n, const double *x, const int incx, const double *y, const int incy)
{
	return ddot_(&n, x, &incx, y, &incy);
}

void SSWAP(const int n, float *X, const int incX, float *Y, const int incY)
{
	sswap_(&n, X, &incX, Y, &incY);
}

void DSWAP(const int n, double *X, const int incX, double *Y, const int incY)
{
	dswap_(&n, X, &incX, Y, &incY);
}


#else
#error "no BLAS lib available..."
#endif
