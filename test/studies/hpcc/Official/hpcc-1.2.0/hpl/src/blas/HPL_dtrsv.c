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

#ifndef HPL_dtrsv

#ifdef HPL_CALL_VSIPL

#ifdef HPL_STDC_HEADERS
static void HPL_dtrsvLNN
(
   const int                  N,
   const double               * A,
   const int                  LDA,
   double                     * X,
   const int                  INCX
)
#else
static void HPL_dtrsvLNN( N, A, LDA, X, INCX )
   const int                  INCX, LDA, N;
   const double               * A;
   double                     * X;
#endif
{
   int                        i, iaij, ix, j, jaj, jx, ldap1 = LDA + 1;
   register double            t0;

   for( j = 0, jaj = 0, jx  = 0; j < N; j++, jaj += ldap1, jx += INCX )
   {
      X[jx] /= A[jaj]; t0 = X[jx];
      for( i = j+1,    iaij  = jaj+1, ix  = jx + INCX;
           i < N; i++, iaij += 1,     ix += INCX ) { X[ix] -= t0 * A[iaij]; }
   }
}

#ifdef HPL_STDC_HEADERS
static void HPL_dtrsvLNU
(
   const int                  N,
   const double               * A,
   const int                  LDA,
   double                     * X,
   const int                  INCX
)
#else
static void HPL_dtrsvLNU( N, A, LDA, X, INCX )
   const int                  INCX, LDA, N;
   const double               * A;
   double                     * X;
#endif
{
   int                        i, iaij, ix, j, jaj, jx, ldap1 = LDA + 1;
   register double            t0;

   for( j = 0, jaj = 0, jx = 0; j < N; j++, jaj += ldap1, jx += INCX )
   {
      t0 = X[jx];
      for( i = j+1,    iaij  = jaj+1, ix  = jx + INCX;
           i < N; i++, iaij += 1,     ix += INCX ) { X[ix] -= t0 * A[iaij]; }
   }
}

#ifdef HPL_STDC_HEADERS
static void HPL_dtrsvLTN
(
   const int                  N,
   const double               * A,
   const int                  LDA,
   double                     * X,
   const int                  INCX
)
#else
static void HPL_dtrsvLTN( N, A, LDA, X, INCX )
   const int                  INCX, LDA, N;
   const double               * A;
   double                     * X;
#endif
{
   int                        i, iaij, ix, j, jaj, jx, ldap1 = LDA + 1;
   register double            t0;

   for( j = N-1,     jaj  = (N-1)*(ldap1), jx  = (N-1)*INCX;
        j >= 0; j--, jaj -= ldap1,         jx -= INCX )
   {
      t0 = X[jx];
      for( i = j+1,    iaij  = 1+jaj, ix  = jx + INCX;
           i < N; i++, iaij += 1,     ix += INCX ) { t0 -= A[iaij] * X[ix]; }
      t0 /= A[jaj]; X[jx] = t0;
   }
}

#ifdef HPL_STDC_HEADERS
static void HPL_dtrsvLTU
(
   const int                  N,
   const double               * A,
   const int                  LDA,
   double                     * X,
   const int                  INCX
)
#else
static void HPL_dtrsvLTU( N, A, LDA, X, INCX )
   const int                  INCX, LDA, N;
   const double               * A;
   double                     * X;
#endif
{
   int                        i, iaij, ix, j, jaj, jx, ldap1 = LDA + 1;
   register double            t0;

   for( j = N-1,     jaj  = (N-1)*(ldap1), jx  = (N-1)*INCX;
        j >= 0; j--, jaj -= ldap1,         jx -= INCX )
   {
      t0 = X[jx];
      for( i = j+1,    iaij  = 1+jaj, ix  = jx + INCX;
           i < N; i++, iaij += 1,     ix += INCX ) { t0 -= A[iaij] * X[ix]; }
      X[jx] = t0;
   }
}


#ifdef HPL_STDC_HEADERS
static void HPL_dtrsvUNN
(
   const int                  N,
   const double               * A,
   const int                  LDA,
   double                     * X,
   const int                  INCX
)
#else
static void HPL_dtrsvUNN( N, A, LDA, X, INCX )
   const int                  INCX, LDA, N;
   const double               * A;
   double                     * X;
#endif
{
   int                        i, iaij, ix, j, jaj, jx;
   register double            t0;

   for( j = N-1,     jaj  = (N-1)*LDA, jx  = (N-1)*INCX;
        j >= 0; j--, jaj -= LDA,       jx -= INCX )
   {
      X[jx] /= A[j+jaj]; t0 = X[jx];
      for( i = 0, iaij = jaj, ix = 0; i < j; i++, iaij += 1, ix += INCX )
      { X[ix] -= t0 * A[iaij]; }
   }
}


#ifdef HPL_STDC_HEADERS
static void HPL_dtrsvUNU
(
   const int                  N,
   const double               * A,
   const int                  LDA,
   double                     * X,
   const int                  INCX
)
#else
static void HPL_dtrsvUNU( N, A, LDA, X, INCX )
   const int                  INCX, LDA, N;
   const double               * A;
   double                     * X;
#endif
{
   int                        i, iaij, ix, j, jaj, jx;
   register double            t0;

   for( j = N-1,     jaj  = (N-1)*LDA, jx  = (N-1)*INCX;
        j >= 0; j--, jaj -= LDA,       jx -= INCX )
   {
      t0 = X[jx];
      for( i = 0, iaij = jaj, ix = 0; i < j; i++, iaij += 1, ix += INCX )
      { X[ix] -= t0 * A[iaij]; }
   }
}


#ifdef HPL_STDC_HEADERS
static void HPL_dtrsvUTN
(
   const int                  N,
   const double               * A,
   const int                  LDA,
   double                     * X,
   const int                  INCX
)
#else
static void HPL_dtrsvUTN( N, A, LDA, X, INCX )
   const int                  INCX, LDA, N;
   const double               * A;
   double                     * X;
#endif
{
   int                        i, iaij, ix, j, jaj, jx;
   register double            t0;

   for( j = 0, jaj = 0,jx = 0; j < N; j++, jaj += LDA, jx += INCX )
   {
      t0 = X[jx];
      for( i = 0, iaij = jaj, ix = 0; i < j; i++, iaij += 1, ix += INCX )
      { t0 -= A[iaij] * X[ix]; }
      t0 /= A[iaij]; X[jx] = t0;
   }
}

#ifdef HPL_STDC_HEADERS
static void HPL_dtrsvUTU
(
   const int                  N,
   const double               * A,
   const int                  LDA,
   double                     * X,
   const int                  INCX
)
#else
static void HPL_dtrsvUTU( N, A, LDA, X, INCX )
   const int                  INCX, LDA, N;
   const double               * A;
   double                     * X;
#endif
{
   int                        i, iaij, ix, j, jaj, jx;
   register double            t0;

   for( j = 0, jaj = 0, jx = 0; j < N; j++, jaj += LDA, jx += INCX )
   {
      t0 = X[jx];
      for( i = 0, iaij = jaj, ix = 0; i < j; i++, iaij += 1, ix += INCX )
      { t0 -= A[iaij] * X[ix]; }
      X[jx] = t0;
   }
}

#ifdef HPL_STDC_HEADERS
static void HPL_dtrsv0
(
   const enum HPL_UPLO        UPLO,
   const enum HPL_TRANS       TRANS,
   const enum HPL_DIAG        DIAG,
   const int                  N,
   const double               * A,
   const int                  LDA,
   double                     * X,
   const int                  INCX
) 
#else
static void HPL_dtrsv0( UPLO, TRANS, DIAG, N, A, LDA, X, INCX )
   const enum HPL_UPLO        UPLO;
   const enum HPL_TRANS       TRANS;
   const enum HPL_DIAG        DIAG;
   const int                  INCX, LDA, N;
   const double               * A;
   double                     * X;
#endif
{
   if( N == 0 ) return;
 
   if( UPLO == HplUpper )
   {
      if( TRANS == HplNoTrans )
      {
         if( DIAG == HplNonUnit ) { HPL_dtrsvUNN( N,    A, LDA, X, INCX ); }
         else                     { HPL_dtrsvUNU( N,    A, LDA, X, INCX ); }
      }
      else
      {
         if( DIAG == HplNonUnit ) { HPL_dtrsvUTN( N,    A, LDA, X, INCX ); }
         else                     { HPL_dtrsvUTU( N,    A, LDA, X, INCX ); }
      }
   }
   else
   {
      if( TRANS == HplNoTrans )
      {
         if( DIAG == HplNonUnit ) { HPL_dtrsvLNN( N,    A, LDA, X, INCX ); }
         else                     { HPL_dtrsvLNU( N,    A, LDA, X, INCX ); }
      }
      else
      {
         if( DIAG == HplNonUnit ) { HPL_dtrsvLTN( N,    A, LDA, X, INCX ); }
         else                     { HPL_dtrsvLTU( N,    A, LDA, X, INCX ); }
      }
   }
}

#endif

#ifdef HPL_STDC_HEADERS
void HPL_dtrsv
(
   const enum HPL_ORDER             ORDER,
   const enum HPL_UPLO              UPLO,
   const enum HPL_TRANS             TRANS,
   const enum HPL_DIAG              DIAG,
   const int                        N,
   const double *                   A,
   const int                        LDA,
   double *                         X,
   const int                        INCX
)
#else
void HPL_dtrsv
( ORDER, UPLO, TRANS, DIAG, N, A, LDA, X, INCX )
   const enum HPL_ORDER             ORDER;
   const enum HPL_UPLO              UPLO;
   const enum HPL_TRANS             TRANS;
   const enum HPL_DIAG              DIAG;
   const int                        N;
   const double *                   A;
   const int                        LDA;
   double *                         X;
   const int                        INCX;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_dtrsv solves one of the systems of equations
 *  
 *     A * x = b,   or   A^T * x = b,
 *  
 * where b and x are n-element vectors and  A  is an n by n non-unit, or
 * unit, upper or lower triangular matrix.
 *  
 * No test for  singularity  or  near-singularity  is included  in  this
 * routine. Such tests must be performed before calling this routine.
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
 * UPLO    (local input)                 const enum HPL_UPLO
 *         On  entry,   UPLO   specifies  whether  the  upper  or  lower
 *         triangular  part  of the array  A  is to be referenced.  When
 *         UPLO==HplUpper, only  the upper triangular part of A is to be
 *         referenced, otherwise only the lower triangular part of A is 
 *         to be referenced. 
 *
 * TRANS   (local input)                 const enum HPL_TRANS
 *         On entry,  TRANS  specifies  the equations  to  be  solved as
 *         follows:
 *            TRANS==HplNoTrans     A   * x = b,
 *            TRANS==HplTrans       A^T * x = b.
 *
 * DIAG    (local input)                 const enum HPL_DIAG
 *         On entry,  DIAG  specifies  whether  A  is unit triangular or
 *         not. When DIAG==HplUnit,  A is assumed to be unit triangular,
 *         and otherwise, A is not assumed to be unit triangular.
 *
 * N       (local input)                 const int
 *         On entry, N specifies the order of the matrix A. N must be at
 *         least zero.
 *
 * A       (local input)                 const double *
 *         On entry,  A  points  to an array of size equal to or greater
 *         than LDA * n. Before entry with  UPLO==HplUpper,  the leading
 *         n by n upper triangular  part of the array A must contain the
 *         upper triangular  matrix and the  strictly  lower  triangular
 *         part of A is not referenced.  When  UPLO==HplLower  on entry,
 *         the  leading n by n lower triangular part of the array A must
 *         contain the lower triangular matrix  and  the  strictly upper
 *         triangular part of A is not referenced.
 *          
 *         Note  that  when  DIAG==HplUnit,  the diagonal elements of  A
 *         not referenced  either,  but are assumed to be unity.
 *
 * LDA     (local input)                 const int
 *         On entry,  LDA  specifies  the  leading  dimension  of  A  as
 *         declared  in  the  calling  (sub) program.  LDA  must  be  at
 *         least MAX(1,n).
 *
 * X       (local input/output)          double *
 *         On entry,  X  is an incremented array of dimension  at  least
 *         ( 1 + ( n - 1 ) * abs( INCX ) )  that  contains the vector x.
 *         Before entry,  the  incremented array  X  must contain  the n
 *         element right-hand side vector b. On exit,  X  is overwritten
 *         with the solution vector x.
 *
 * INCX    (local input)                 const int
 *         On entry, INCX specifies the increment for the elements of X.
 *         INCX must not be zero.
 *
 * ---------------------------------------------------------------------
 */ 
#ifdef HPL_CALL_CBLAS
   cblas_dtrsv( ORDER, UPLO, TRANS, DIAG, N, A, LDA, X, INCX );
#endif
#ifdef HPL_CALL_VSIPL
   if( ORDER == HplColumnMajor )
   {
      HPL_dtrsv0( UPLO, TRANS, DIAG, N, A, LDA, X, INCX );
   }
   else
   {
      HPL_dtrsv0( ( UPLO  == HplUpper   ? HplLower : HplUpper   ),
                  ( TRANS == HplNoTrans ? HplTrans : HplNoTrans ),
                  DIAG, N, A, LDA, X, INCX );
   }
#endif
#ifdef HPL_CALL_FBLAS
#ifdef StringSunStyle
#ifdef HPL_USE_F77_INTEGER_DEF
   F77_INTEGER               IONE = 1;
#else
   int                       IONE = 1;
#endif
#endif
#ifdef StringStructVal
   F77_CHAR                  fuplo, ftran, fdiag;
#endif
#ifdef StringStructPtr
   F77_CHAR                  fuplo, ftran, fdiag;
#endif
#ifdef StringCrayStyle
   F77_CHAR                  fuplo, ftran, fdiag;
#endif
 
#ifdef HPL_USE_F77_INTEGER_DEF 
   const F77_INTEGER         F77N = N, F77lda = LDA, F77incx = INCX;
#else
#define F77N              N
#define F77lda            LDA
#define F77incx           INCX
#endif
   char                      cuplo, ctran, cdiag;

   if( ORDER == HplColumnMajor )
   {
      cuplo = ( UPLO  == HplUpper   ? 'U' : 'L' );
      ctran = ( TRANS == HplNoTrans ? 'N' : 'T' );
   }
   else
   {
      cuplo = ( UPLO  == HplUpper   ? 'L' : 'U' );
      ctran = ( TRANS == HplNoTrans ? 'T' : 'N' );
   }
   cdiag = ( DIAG == HplNonUnit ? 'N' : 'U' );

#ifdef StringSunStyle
   F77dtrsv( &cuplo, &ctran, &cdiag, &F77N, A, &F77lda, X, &F77incx,
             IONE, IONE, IONE );
#endif
#ifdef StringCrayStyle
   ftran = HPL_C2F_CHAR( ctran ); fdiag = HPL_C2F_CHAR( cdiag );
   fuplo = HPL_C2F_CHAR( cuplo );
   F77dtrsv( fuplo,  ftran,  fdiag,  &F77N, A, &F77lda, X, &F77incx );
#endif
#ifdef StringStructVal
   fuplo.len = 1; fuplo.cp = &cuplo; ftran.len = 1; ftran.cp = &ctran;
   fdiag.len = 1; fdiag.cp = &cdiag;
   F77dtrsv( fuplo,  ftran,  fdiag,  &F77N, A, &F77lda, X, &F77incx );
#endif
#ifdef StringStructPtr
   fuplo.len = 1; fuplo.cp = &cuplo; ftran.len = 1; ftran.cp = &ctran;
   fdiag.len = 1; fdiag.cp = &cdiag;
   F77dtrsv( &fuplo, &ftran, &fdiag, &F77N, A, &F77lda, X, &F77incx );
#endif

#endif
/*
 * End of HPL_dtrsv
 */
}

#endif
