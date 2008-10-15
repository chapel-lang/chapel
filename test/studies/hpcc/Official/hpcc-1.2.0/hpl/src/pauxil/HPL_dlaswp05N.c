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
/*
 * Define default value for unrolling factor
 */
#ifndef HPL_LASWP05N_DEPTH
#define    HPL_LASWP05N_DEPTH       32
#define    HPL_LASWP05N_LOG2_DEPTH   5
#endif

#ifdef HPL_STDC_HEADERS
void HPL_dlaswp05N
(
   const int                        M,
   const int                        N,
   double *                         A,
   const int                        LDA,
   const double *                   U,
   const int                        LDU,
   const int *                      LINDXA,
   const int *                      LINDXAU
)
#else
void HPL_dlaswp05N
( M, N, A, LDA, U, LDU, LINDXA, LINDXAU )
   const int                        M;
   const int                        N;
   double *                         A;
   const int                        LDA;
   const double *                   U;
   const int                        LDU;
   const int *                      LINDXA;
   const int *                      LINDXAU;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_dlaswp05N copies rows of  U of global offset LINDXAU into rows of
 * A at positions indicated by LINDXA.
 *
 * Arguments
 * =========
 *
 * M       (local input)                 const int
 *         On entry, M  specifies the number of rows of U that should be
 *         copied into A. M must be at least zero.
 *
 * N       (local input)                 const int
 *         On entry, N specifies the length of the rows of U that should
 *         be copied into A. N must be at least zero.
 *
 * A       (local output)                double *
 *         On entry, A points to an array of dimension (LDA,N). On exit,
 *         the  rows of this array specified by  LINDXA  are replaced by
 *         rows of U indicated by LINDXAU.
 *
 * LDA     (local input)                 const int
 *         On entry, LDA specifies the leading dimension of the array A.
 *         LDA must be at least MAX(1,M).
 *
 * U       (local input/output)          const double *
 *         On entry,  U  points to an array of dimension  (LDU,N).  This
 *         array contains the rows that are to be copied into A.
 *
 * LDU     (local input)                 const int
 *         On entry, LDU specifies the leading dimension of the array U.
 *         LDU must be at least MAX(1,M).
 *
 * LINDXA  (local input)                 const int *
 *         On entry, LINDXA is an array of dimension M that contains the
 *         local row indexes of A that should be copied from U.
 *
 * LINDXAU (local input)                 const int *
 *         On entry, LINDXAU  is an array of dimension  M that  contains
 *         the local row indexes of U that should be copied in A.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   const double               * U0 = U, * u0;
   double                     * a0;
   const int                  incA = (int)( (unsigned int)(LDA) <<
                                            HPL_LASWP05N_LOG2_DEPTH ),
                              incU = (int)( (unsigned int)(LDU) <<
                                            HPL_LASWP05N_LOG2_DEPTH );
   int                        nr, nu;
   register int               i, j;
/* ..
 * .. Executable Statements ..
 */
   if( ( M <= 0 ) || ( N <= 0 ) ) return;

   nr = N - ( nu = (int)( ( (unsigned int)(N) >> HPL_LASWP05N_LOG2_DEPTH ) <<
                            HPL_LASWP05N_LOG2_DEPTH ) );

   for( j = 0; j < nu; j += HPL_LASWP05N_DEPTH, A += incA, U0 += incU )
   {
      for( i = 0; i < M; i++ )
      {
         a0 = A + LINDXA[i]; u0 = U0 + LINDXAU[i];

         *a0 = *u0; a0 += LDA; u0 += LDU;
#if ( HPL_LASWP05N_DEPTH >  1 )
         *a0 = *u0; a0 += LDA; u0 += LDU;
#endif
#if ( HPL_LASWP05N_DEPTH >  2 )
         *a0 = *u0; a0 += LDA; u0 += LDU; *a0 = *u0; a0 += LDA; u0 += LDU;
#endif
#if ( HPL_LASWP05N_DEPTH >  4 )
         *a0 = *u0; a0 += LDA; u0 += LDU; *a0 = *u0; a0 += LDA; u0 += LDU;
         *a0 = *u0; a0 += LDA; u0 += LDU; *a0 = *u0; a0 += LDA; u0 += LDU;
#endif
#if ( HPL_LASWP05N_DEPTH >  8 )
         *a0 = *u0; a0 += LDA; u0 += LDU; *a0 = *u0; a0 += LDA; u0 += LDU;
         *a0 = *u0; a0 += LDA; u0 += LDU; *a0 = *u0; a0 += LDA; u0 += LDU;
         *a0 = *u0; a0 += LDA; u0 += LDU; *a0 = *u0; a0 += LDA; u0 += LDU;
         *a0 = *u0; a0 += LDA; u0 += LDU; *a0 = *u0; a0 += LDA; u0 += LDU;
#endif
#if ( HPL_LASWP05N_DEPTH > 16 )
         *a0 = *u0; a0 += LDA; u0 += LDU; *a0 = *u0; a0 += LDA; u0 += LDU;
         *a0 = *u0; a0 += LDA; u0 += LDU; *a0 = *u0; a0 += LDA; u0 += LDU;
         *a0 = *u0; a0 += LDA; u0 += LDU; *a0 = *u0; a0 += LDA; u0 += LDU;
         *a0 = *u0; a0 += LDA; u0 += LDU; *a0 = *u0; a0 += LDA; u0 += LDU;
         *a0 = *u0; a0 += LDA; u0 += LDU; *a0 = *u0; a0 += LDA; u0 += LDU;
         *a0 = *u0; a0 += LDA; u0 += LDU; *a0 = *u0; a0 += LDA; u0 += LDU;
         *a0 = *u0; a0 += LDA; u0 += LDU; *a0 = *u0; a0 += LDA; u0 += LDU;
         *a0 = *u0; a0 += LDA; u0 += LDU; *a0 = *u0; a0 += LDA; u0 += LDU;
#endif
      }
   }

   if( nr )
   {
      for( i = 0; i < M; i++ )
      {
         a0 = A + LINDXA[i]; u0 = U0 + LINDXAU[i];
         for( j = 0; j < nr; j++, a0 += LDA, u0 += LDU ) { *a0 = *u0; }
      }
   }
/*
 * End of HPL_dlaswp05N
 */
}
