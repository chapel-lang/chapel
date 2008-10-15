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

#ifdef HPL_STDC_HEADERS
double HPL_dlange
(
   const HPL_T_NORM                 NORM,
   const int                        M,
   const int                        N,
   const double *                   A,
   const int                        LDA
)
#else
double HPL_dlange
( NORM, M, N, A, LDA )
   const HPL_T_NORM                 NORM;
   const int                        M;
   const int                        N;
   const double *                   A;
   const int                        LDA;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_dlange returns  the value of the one norm,  or the infinity norm,
 * or the element of largest absolute value of a matrix A:              
 *  
 *    max(abs(A(i,j))) when NORM = HPL_NORM_A,                          
 *    norm1(A),        when NORM = HPL_NORM_1,                          
 *    normI(A),        when NORM = HPL_NORM_I,                          
 *  
 * where norm1 denotes the one norm of a matrix (maximum column sum) and
 * normI denotes  the infinity norm of a matrix (maximum row sum).  Note
 * that max(abs(A(i,j))) is not a matrix norm.
 *
 * Arguments
 * =========
 *
 * NORM    (local input)                 const HPL_T_NORM
 *         On entry,  NORM  specifies  the  value to be returned by this
 *         function as described above.
 *
 * M       (local input)                 const int
 *         On entry,  M  specifies  the number  of rows of the matrix A.
 *         M must be at least zero.
 *
 * N       (local input)                 const int
 *         On entry,  N specifies the number of columns of the matrix A.
 *         N must be at least zero.
 *
 * A       (local input)                 const double *
 *         On entry,  A  points to an  array of dimension  (LDA,N), that
 *         contains the matrix A.
 *
 * LDA     (local input)                 const int
 *         On entry, LDA specifies the leading dimension of the array A.
 *         LDA must be at least max(1,M).
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   double                     s, v0=HPL_rzero, * work = NULL;
   int                        i, j;
/* ..
 * .. Executable Statements ..
 */
   if( ( M <= 0 ) || ( N <= 0 ) ) return( HPL_rzero );

   if(      NORM == HPL_NORM_A )
   {
/*
 * max( abs( A ) )
 */
      for( j = 0; j < N; j++ )
      {
         for( i = 0; i < M; i++ ) { v0 = Mmax( v0, Mabs( *A ) ); A++; }
         A += LDA - M;
      }
   }
   else if( NORM == HPL_NORM_1 )
   {
/*
 * Find norm_1( A ).
 */
      work = (double*)malloc( N * sizeof( double ) );
      if( work == NULL )
      { HPL_abort( __LINE__, "HPL_dlange", "Memory allocation failed" ); }
      else
      {
         for( j = 0; j < N; j++ )
         {
            s = HPL_rzero;
            for( i = 0; i < M; i++ ) { s += Mabs( *A ); A++; }
            work[j] = s; A += LDA - M;
         }
/*
 * Find maximum sum of columns for 1-norm
 */
         v0 = work[HPL_idamax( N, work, 1 )]; v0 = Mabs( v0 );
         if( work ) free( work );
      }
   }
   else if( NORM == HPL_NORM_I )
   {
/*
 * Find norm_inf( A )
 */
      work = (double*)malloc( M * sizeof( double ) );
      if( work == NULL )
      { HPL_abort( __LINE__, "HPL_dlange", "Memory allocation failed" ); }
      else
      {
         for( i = 0; i < M; i++ ) { work[i] = HPL_rzero; }

         for( j = 0; j < N; j++ )
         {
            for( i = 0; i < M; i++ ) { work[i] += Mabs( *A ); A++; }
            A += LDA - M;
         }
/*       
 * Find maximum sum of rows for inf-norm
 */      
         v0 = work[HPL_idamax( M, work, 1 )]; v0 = Mabs( v0 );
         if( work ) free( work );
      }
   }

   return( v0 );
/*
 * End of HPL_dlange
 */
} 
