/* 
 * -- High Performance Computing Linpack Benchmark (HPL)                
 *    HPL - 1.0a - January 20, 2004                          
 *    Antoine P. Petitet                                                
 *    University of Tennessee, Knoxville                                
 *    Innovative Computing Laboratories                                 
 *    (C) Copyright 2000-2004 All Rights Reserved                       
 *                                                                      
 * -- Copyright notice and Licensing terms:                             
 *                                                                      
 * Redistribution  and  use in  source and binary forms, with or without
 * modification, are  permitted provided  that the following  conditions
 * are met:                                                             
 *                                                                      
 * 1. Redistributions  of  source  code  must retain the above copyright
 * notice, this list of conditions and the following disclaimer.        
 *                                                                      
 * 2. Redistributions in binary form must reproduce  the above copyright
 * notice, this list of conditions,  and the following disclaimer in the
 * documentation and/or other materials provided with the distribution. 
 *                                                                      
 * 3. All  advertising  materials  mentioning  features  or  use of this
 * software must display the following acknowledgement:                 
 * This  product  includes  software  developed  at  the  University  of
 * Tennessee, Knoxville, Innovative Computing Laboratories.             
 *                                                                      
 * 4. The name of the  University,  the name of the  Laboratory,  or the
 * names  of  its  contributors  may  not  be used to endorse or promote
 * products  derived   from   this  software  without  specific  written
 * permission.                                                          
 *                                                                      
 * -- Disclaimer:                                                       
 *                                                                      
 * THIS  SOFTWARE  IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,  INCLUDING,  BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY
 * OR  CONTRIBUTORS  BE  LIABLE FOR ANY  DIRECT,  INDIRECT,  INCIDENTAL,
 * SPECIAL,  EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES  (INCLUDING,  BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA OR PROFITS; OR BUSINESS INTERRUPTION)  HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT,  STRICT LIABILITY,  OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 * ---------------------------------------------------------------------
 */ 
/*
 * Include files
 */
#include "hpl.h"

#ifndef HPL_dger

#ifdef HPL_STDC_HEADERS
void HPL_dger
(
   const enum HPL_ORDER             ORDER,
   const int                        M,
   const int                        N,
   const double                     ALPHA,
   const double *                   X,
   const int                        INCX,
   double *                         Y,
   const int                        INCY,
   double *                         A,
   const int                        LDA
)
#else
void HPL_dger
( ORDER, M, N, ALPHA, X, INCX, Y, INCY, A, LDA )
   const enum HPL_ORDER             ORDER;
   const int                        M;
   const int                        N;
   const double                     ALPHA;
   const double *                   X;
   const int                        INCX;
   double *                         Y;
   const int                        INCY;
   double *                         A;
   const int                        LDA;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_dger performs the rank 1 operation
 *  
 *     A := alpha * x * y^T + A,
 *  
 * where alpha is a scalar,  x is an m-element vector, y is an n-element
 * vector and A is an m by n matrix.
 *
 * Arguments
 * =========
 *
 * ORDER   (local input)                 const enum HPL_ORDER
 *         On entry, ORDER  specifies the storage format of the operands
 *         as follows:                                                  
 *            ORDER = HplRowMajor,                                      
 *            ORDER = HplColumnMajor.                                   
 *
 * M       (local input)                 const int
 *         On entry,  M  specifies  the number of rows of  the matrix A.
 *         M must be at least zero.
 *
 * N       (local input)                 const int
 *         On entry, N  specifies the number of columns of the matrix A.
 *         N must be at least zero.
 *
 * ALPHA   (local input)                 const double
 *         On entry, ALPHA specifies the scalar alpha.   When  ALPHA  is
 *         supplied as zero then  X and Y  need not be set on input.
 *
 * X       (local input)                 const double *
 *         On entry,  X  is an incremented array of dimension  at  least
 *         ( 1 + ( m - 1 ) * abs( INCX ) )  that  contains the vector x.
 *
 * INCX    (local input)                 const int
 *         On entry, INCX specifies the increment for the elements of X.
 *         INCX must not be zero.
 *
 * Y       (local input)                 double *
 *         On entry,  Y  is an incremented array of dimension  at  least
 *         ( 1 + ( n - 1 ) * abs( INCY ) )  that  contains the vector y.
 *
 * INCY    (local input)                 const int
 *         On entry, INCY specifies the increment for the elements of Y.
 *         INCY must not be zero.
 *
 * A       (local input/output)          double *
 *         On entry,  A  points  to an array of size equal to or greater
 *         than LDA * n.  Before  entry, the leading m by n part  of the
 *         array  A  must contain the matrix coefficients. On exit, A is
 *         overwritten by the updated matrix.
 *
 * LDA     (local input)                 const int
 *         On entry,  LDA  specifies  the  leading  dimension  of  A  as
 *         declared  in  the  calling  (sub) program.  LDA  must  be  at
 *         least MAX(1,m).
 *
 * ---------------------------------------------------------------------
 */ 
#ifdef HPL_CALL_CBLAS
   cblas_dger( ORDER, M, N, ALPHA, X, INCX, Y, INCY, A, LDA );
#endif
#ifdef HPL_CALL_VSIPL
   register double           t0;
   int                       i, iaij, ix, iy, j, jaj, jx, jy;

   if( ( M == 0 ) || ( N == 0 ) || ( ALPHA == HPL_rzero ) ) return;
 
   if( ORDER == HplColumnMajor )
   {
      for( j = 0, jaj = 0, jy = 0; j < N; j++, jaj += LDA, jy += INCY )
      {
         t0 = ALPHA * Y[jy];
         for( i = 0, iaij = jaj, ix = 0; i < M; i++, iaij += 1, ix += INCX )
         { A[iaij] += X[ix] * t0; }
      }
   }
   else
   {
      for( j = 0, jaj = 0, jx = 0; j < M; j++, jaj += LDA, jx += INCX )
      {
         t0 = ALPHA * X[jx];
         for( i = 0, iaij = jaj, iy = 0; i < N; i++, iaij += 1, iy += INCY )
         { A[iaij] += Y[iy] * t0; }
      }
   }
#endif
#ifdef HPL_CALL_FBLAS
   double                    alpha = ALPHA;
#ifdef HPL_USE_F77_INTEGER_DEF
   const F77_INTEGER         F77M    = M,   F77N    = N,
                             F77lda  = LDA, F77incx = INCX, F77incy = INCY;
#else
#define F77M                 M
#define F77N                 N
#define F77lda               LDA
#define F77incx              INCX
#define F77incy              INCY
#endif

   if( ORDER == HplColumnMajor )
   {  F77dger( &F77M, &F77N, &alpha, X, &F77incx, Y, &F77incy, A, &F77lda ); }
   else
   {  F77dger( &F77N, &F77M, &alpha, Y, &F77incy, X, &F77incx, A, &F77lda ); }
#endif
/*
 * End of HPL_dger
 */
}

#endif
