/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
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

#ifndef __BLAS_H__
#define __BLAS_H__

#include <starpu.h>

#ifdef STARPU_ATLAS
#include <cblas.h>
#endif

void SGEMM(char *transa, char *transb, int M, int N, int K, float alpha, float *A, int lda, 
		float *B, int ldb, float beta, float *C, int ldc);
void DGEMM(char *transa, char *transb, int M, int N, int K, double alpha, double *A, int lda, 
		double *B, int ldb, double beta, double *C, int ldc);
void SGEMV(char *transa, int M, int N, float alpha, float *A, int lda,
		float *X, int incX, float beta, float *Y, int incY);
void DGEMV(char *transa, int M, int N, double alpha, double *A, int lda,
		double *X, int incX, double beta, double *Y, int incY);
float SASUM(int N, float *X, int incX);
double DASUM(int N, double *X, int incX);
void SSCAL(int N, float alpha, float *X, int incX);
void DSCAL(int N, double alpha, double *X, int incX);
void STRSM (const char *side, const char *uplo, const char *transa,
                   const char *diag, const int m, const int n,
                   const float alpha, const float *A, const int lda,
                   float *B, const int ldb);
void DTRSM (const char *side, const char *uplo, const char *transa,
                   const char *diag, const int m, const int n,
                   const double alpha, const double *A, const int lda,
                   double *B, const int ldb);
void DGEMM(char *transa, char *transb, int M, int N, int K, 
			double alpha, double *A, int lda, double *B, int ldb, 
			double beta, double *C, int ldc);
void SSYR (const char *uplo, const int n, const float alpha,
                  const float *x, const int incx, float *A, const int lda);
void SSYRK (const char *uplo, const char *trans, const int n,
                   const int k, const float alpha, const float *A,
                   const int lda, const float beta, float *C,
                   const int ldc);
void SGER (const int m, const int n, const float alpha,
                  const float *x, const int incx, const float *y,
                  const int incy, float *A, const int lda);
void DGER(const int m, const int n, const double alpha,
                  const double *x, const int incx, const double *y,
                  const int incy, double *A, const int lda);
void STRSV (const char *uplo, const char *trans, const char *diag, 
                   const int n, const float *A, const int lda, float *x, 
                   const int incx);
void STRMM(const char *side, const char *uplo, const char *transA,
                 const char *diag, const int m, const int n,
                 const float alpha, const float *A, const int lda,
                 float *B, const int ldb);
void DTRMM(const char *side, const char *uplo, const char *transA,
                 const char *diag, const int m, const int n,
                 const double alpha, const double *A, const int lda,
                 double *B, const int ldb);
void STRMV(const char *uplo, const char *transA, const char *diag,
                 const int n, const float *A, const int lda, float *X,
                 const int incX);
void SAXPY(const int n, const float alpha, float *X, const int incX, float *Y, const int incy);
void DAXPY(const int n, const double alpha, double *X, const int incX, double *Y, const int incY);
int ISAMAX (const int n, float *X, const int incX);
int IDAMAX (const int n, double *X, const int incX);
float SDOT(const int n, const float *x, const int incx, const float *y, const int incy);
double DDOT(const int n, const double *x, const int incx, const double *y, const int incy);
void SSWAP(const int n, float *x, const int incx, float *y, const int incy);
void DSWAP(const int n, double *x, const int incx, double *y, const int incy);

#if defined(STARPU_GOTO) || defined(STARPU_SYSTEM_BLAS) || defined(STARPU_MKL)

extern void sgemm_ (const char *transa, const char *transb, const int *m,
                   const int *n, const int *k, const float *alpha, 
                   const float *A, const int *lda, const float *B, 
                   const int *ldb, const float *beta, float *C, 
                   const int *ldc);
extern void dgemm_ (const char *transa, const char *transb, const int *m,
                   const int *n, const int *k, const double *alpha, 
                   const double *A, const int *lda, const double *B, 
                   const int *ldb, const double *beta, double *C, 
                   const int *ldc);
extern void sgemv_(const char *trans, int *m, int *n, float *alpha,
                   void *a, int *lda, void *x, int *incx, 
                   float *beta, void *y, int *incy);
extern void dgemv_(const char *trans, int *m, int *n, double *alpha,
                   void *a, int *lda, void *x, int *incx,
                   double *beta, void *y, int *incy);
extern void ssyr_ (const char *uplo, const int *n, const float *alpha,
                  const float *x, const int *incx, float *A, const int *lda);
extern void ssyrk_ (const char *uplo, const char *trans, const int *n,
                   const int *k, const float *alpha, const float *A,
                   const int *lda, const float *beta, float *C,
                   const int *ldc);
extern void strsm_ (const char *side, const char *uplo, const char *transa, 
                   const char *diag, const int *m, const int *n,
                   const float *alpha, const float *A, const int *lda,
                   float *B, const int *ldb);
extern void dtrsm_ (const char *side, const char *uplo, const char *transa, 
                   const char *diag, const int *m, const int *n,
                   const double *alpha, const double *A, const int *lda,
                   double *B, const int *ldb);
extern double sasum_ (const int *n, const float *x, const int *incx);
extern double dasum_ (const int *n, const double *x, const int *incx);
extern void sscal_ (const int *n, const float *alpha, float *x,
                   const int *incx);
extern void dscal_ (const int *n, const double *alpha, double *x,
                   const int *incx);
extern void sger_(const int *m, const int *n, const float *alpha,
                  const float *x, const int *incx, const float *y,
                  const int *incy, float *A, const int *lda);
extern void dger_(const int *m, const int *n, const double *alpha,
                  const double *x, const int *incx, const double *y,
                  const int *incy, double *A, const int *lda);
extern void strsv_ (const char *uplo, const char *trans, const char *diag, 
                   const int *n, const float *A, const int *lda, float *x, 
                   const int *incx);
extern void strmm_(const char *side, const char *uplo, const char *transA,
                 const char *diag, const int *m, const int *n,
                 const float *alpha, const float *A, const int *lda,
                 float *B, const int *ldb);
extern void dtrmm_(const char *side, const char *uplo, const char *transA,
                 const char *diag, const int *m, const int *n,
                 const double *alpha, const double *A, const int *lda,
                 double *B, const int *ldb);
extern void strmv_(const char *uplo, const char *transA, const char *diag,
                 const int *n, const float *A, const int *lda, float *X,
                 const int *incX);
extern void saxpy_(const int *n, const float *alpha, float *X, const int *incX,
		float *Y, const int *incy);
extern void daxpy_(const int *n, const double *alpha, double *X, const int *incX,
		double *Y, const int *incy);
extern int isamax_(const int *n, float *X, const int *incX);
extern int idamax_(const int *n, double *X, const int *incX);
/* for some reason, FLOATRET is not a float but a double in GOTOBLAS */
extern double sdot_(const int *n, const float *x, const int *incx, const float *y, const int *incy);
extern double ddot_(const int *n, const double *x, const int *incx, const double *y, const int *incy);
extern void sswap_(const int *n, float *x, const int *incx, float *y, const int *incy);
extern void dswap_(const int *n, double *x, const int *incx, double *y, const int *incy);

#endif

#endif /* __BLAS_H__ */
