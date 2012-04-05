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
#if defined(STARPU_MKL)
#define MKLcomplex8 complex float
#define MKLcomplex16 complex double
#endif

void CGEMM(char *transa, char *transb, int M, int N, int K, complex float alpha, complex float *A, int lda, 
		complex float *B, int ldb, complex float beta, complex float *C, int ldc);
void ZGEMM(char *transa, char *transb, int M, int N, int K, complex double alpha, complex double *A, int lda, 
		complex double *B, int ldb, complex double beta, complex double *C, int ldc);
void CGEMV(char *transa, int M, int N, complex float alpha, complex float *A, int lda,
		complex float *X, int incX, complex float beta, complex float *Y, int incY);
void ZGEMV(char *transa, int M, int N, complex double alpha, complex double *A, int lda,
		complex double *X, int incX, complex double beta, complex double *Y, int incY);
float SCASUM(int N, complex float *X, int incX);
double DZASUM(int N, complex double *X, int incX);
void CSCAL(int N, complex float alpha, complex float *X, int incX);
void ZSCAL(int N, complex double alpha, complex double *X, int incX);
void CTRSM (const char *side, const char *uplo, const char *transa,
                   const char *diag, const int m, const int n,
                   const complex float alpha, const complex float *A, const int lda,
                   complex float *B, const int ldb);
void ZTRSM (const char *side, const char *uplo, const char *transa,
                   const char *diag, const int m, const int n,
                   const complex double alpha, const complex double *A, const int lda,
                   complex double *B, const int ldb);
void CSYR (const char *uplo, const int n, const complex float alpha,
                  const complex float *x, const int incx, complex float *A, const int lda);
void CSYRK (const char *uplo, const char *trans, const int n,
                   const int k, const complex float alpha, const complex float *A,
                   const int lda, const complex float beta, complex float *C,
                   const int ldc);
void CGERU (const int m, const int n, const complex float alpha,
                  const complex float *x, const int incx, const complex float *y,
                  const int incy, complex float *A, const int lda);
void ZGERU(const int m, const int n, const complex double alpha,
                  const complex double *x, const int incx, const complex double *y,
                  const int incy, complex double *A, const int lda);
void CTRSV (const char *uplo, const char *trans, const char *diag, 
                   const int n, const complex float *A, const int lda, complex float *x, 
                   const int incx);
void CTRMM(const char *side, const char *uplo, const char *transA,
                 const char *diag, const int m, const int n,
                 const complex float alpha, const complex float *A, const int lda,
                 complex float *B, const int ldb);
void ZTRMM(const char *side, const char *uplo, const char *transA,
                 const char *diag, const int m, const int n,
                 const complex double alpha, const complex double *A, const int lda,
                 complex double *B, const int ldb);
void CTRMV(const char *uplo, const char *transA, const char *diag,
                 const int n, const complex float *A, const int lda, complex float *X,
                 const int incX);
void CAXPY(const int n, const complex float alpha, complex float *X, const int incX, complex float *Y, const int incy);
void ZAXPY(const int n, const complex double alpha, complex double *X, const int incX, complex double *Y, const int incY);
int ICAMAX (const int n, complex float *X, const int incX);
int IZAMAX (const int n, complex double *X, const int incX);
complex float CDOTU(const int n, const complex float *x, const int incx, const complex float *y, const int incy);
complex double ZDOTU(const int n, const complex double *x, const int incx, const complex double *y, const int incy);
void CSWAP(const int n, complex float *x, const int incx, complex float *y, const int incy);
void ZSWAP(const int n, complex double *x, const int incx, complex double *y, const int incy);

#if defined(STARPU_GOTO) || defined(STARPU_SYSTEM_BLAS)
#error not implemented
#elif defined(STARPU_MKL)

extern void cgemm_ (const char *transa, const char *transb, const int *m,
                   const int *n, const int *k, const complex float *alpha, 
                   const complex float *A, const int *lda, const complex float *B, 
                   const int *ldb, const complex float *beta, complex float *C, 
                   const int *ldc);
extern void zgemm_ (const char *transa, const char *transb, const int *m,
                   const int *n, const int *k, const complex double *alpha, 
                   const complex double *A, const int *lda, const complex double *B, 
                   const int *ldb, const complex double *beta, complex double *C, 
                   const int *ldc);
extern void cgemv_(const char *trans, int *m, int *n, complex float *alpha,
                   void *a, int *lda, void *x, int *incx, 
                   complex float *beta, void *y, int *incy);
extern void zgemv_(const char *trans, int *m, int *n, complex double *alpha,
                   void *a, int *lda, void *x, int *incx,
                   complex double *beta, void *y, int *incy);
extern void csyr_ (const char *uplo, const int *n, const complex float *alpha,
                  const complex float *x, const int *incx, complex float *A, const int *lda);
extern void csyrk_ (const char *uplo, const char *trans, const int *n,
                   const int *k, const complex float *alpha, const complex float *A,
                   const int *lda, const complex float *beta, complex float *C,
                   const int *ldc);
extern void ctrsm_ (const char *side, const char *uplo, const char *transa, 
                   const char *diag, const int *m, const int *n,
                   const complex float *alpha, const complex float *A, const int *lda,
                   complex float *B, const int *ldb);
extern void ztrsm_ (const char *side, const char *uplo, const char *transa, 
                   const char *diag, const int *m, const int *n,
                   const complex double *alpha, const complex double *A, const int *lda,
                   complex double *B, const int *ldb);
extern complex double scasum_ (const int *n, const complex float *x, const int *incx);
extern complex double dzasum_ (const int *n, const complex double *x, const int *incx);
extern void cscal_ (const int *n, const complex float *alpha, complex float *x,
                   const int *incx);
extern void zscal_ (const int *n, const complex double *alpha, complex double *x,
                   const int *incx);
extern void cgeru_(const int *m, const int *n, const complex float *alpha,
                  const complex float *x, const int *incx, const complex float *y,
                  const int *incy, complex float *A, const int *lda);
extern void zgeru_(const int *m, const int *n, const complex double *alpha,
                  const complex double *x, const int *incx, const complex double *y,
                  const int *incy, complex double *A, const int *lda);
extern void ctrsv_ (const char *uplo, const char *trans, const char *diag, 
                   const int *n, const complex float *A, const int *lda, complex float *x, 
                   const int *incx);
extern void ctrmm_(const char *side, const char *uplo, const char *transA,
                 const char *diag, const int *m, const int *n,
                 const complex float *alpha, const complex float *A, const int *lda,
                 complex float *B, const int *ldb);
extern void ztrmm_(const char *side, const char *uplo, const char *transA,
                 const char *diag, const int *m, const int *n,
                 const complex double *alpha, const complex double *A, const int *lda,
                 complex double *B, const int *ldb);
extern void ctrmv_(const char *uplo, const char *transA, const char *diag,
                 const int *n, const complex float *A, const int *lda, complex float *X,
                 const int *incX);
extern void caxpy_(const int *n, const complex float *alpha, complex float *X, const int *incX,
		complex float *Y, const int *incy);
extern void zaxpy_(const int *n, const complex double *alpha, complex double *X, const int *incX,
		complex double *Y, const int *incy);
extern int icamax_(const int *n, complex float *X, const int *incX);
extern int izamax_(const int *n, complex double *X, const int *incX);
/* for some reason, FLOATRET is not a float but a double in GOTOBLAS */
extern complex double cdotu_(const int *n, const complex float *x, const int *incx, const complex float *y, const int *incy);
extern complex double zdotu_(const int *n, const complex double *x, const int *incx, const complex double *y, const int *incy);
extern void cswap_(const int *n, complex float *x, const int *incx, complex float *y, const int *incy);
extern void zswap_(const int *n, complex double *x, const int *incx, complex double *y, const int *incy);

#endif

#endif /* __BLAS_COMPLEX_H__ */
