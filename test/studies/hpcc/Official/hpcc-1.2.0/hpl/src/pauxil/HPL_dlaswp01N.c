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
#ifndef HPL_LASWP01N_DEPTH
#define    HPL_LASWP01N_DEPTH      32
#define    HPL_LASWP01N_LOG2_DEPTH  5
#endif

#ifdef HPL_STDC_HEADERS
void HPL_dlaswp01N
(
   const int                        M,
   const int                        N,
   double *                         A,
   const int                        LDA,
   double *                         U,
   const int                        LDU,
   const int *                      LINDXA,
   const int *                      LINDXAU
)
#else
void HPL_dlaswp01N
( M, N, A, LDA, U, LDU, LINDXA, LINDXAU )
   const int                        M;
   const int                        N;
   double *                         A;
   const int                        LDA;
   double *                         U;
   const int                        LDU;
   const int *                      LINDXA;
   const int *                      LINDXAU;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_dlaswp01N copies  scattered rows  of  A  into itself  and into an
 * array  U.  The row offsets in  A  of the source rows are specified by
 * LINDXA.  The  destination of those rows are specified by  LINDXAU.  A
 * positive value of  LINDXAU indicates that the array destination is U,
 * and A otherwise.
 *
 * Arguments
 * =========
 *
 * M       (local input)                 const int
 *         On entry, M  specifies the number of rows of A that should be
 *         moved within A or copied into U. M must be at least zero.
 *
 * N       (local input)                 const int
 *         On entry, N  specifies the length of rows of A that should be
 *         moved within A or copied into U. N must be at least zero.
 *
 * A       (local input/output)          double *
 *         On entry, A points to an array of dimension (LDA,N). The rows
 *         of this array specified by LINDXA should be moved within A or
 *         copied into U.
 *
 * LDA     (local input)                 const int
 *         On entry, LDA specifies the leading dimension of the array A.
 *         LDA must be at least MAX(1,M).
 *
 * U       (local input/output)          double *
 *         On entry, U points to an array of dimension (LDU,N). The rows
 *         of A specified by LINDXA are be copied within this array U at
 *         the positions indicated by positive values of LINDXAU.
 *
 * LDU     (local input)                 const int
 *         On entry, LDU specifies the leading dimension of the array U.
 *         LDU must be at least MAX(1,M).
 *
 * LINDXA  (local input)                 const int *
 *         On entry, LINDXA is an array of dimension M that contains the
 *         local  row indexes  of  A  that should be moved within  A  or
 *         or copied into U.
 *
 * LINDXAU (local input)                 const int *
 *         On entry, LINDXAU  is an array of dimension  M that  contains
 *         the local  row indexes of  U  where the rows of  A  should be
 *         copied at. This array also contains the  local row offsets in
 *         A where some of the rows of A should be moved to.  A positive
 *         value of  LINDXAU[i]  indicates that the row  LINDXA[i]  of A
 *         should be copied into U at the position LINDXAU[i]; otherwise
 *         the row  LINDXA[i]  of  A  should be moved  at  the  position
 *         -LINDXAU[i] within A.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   double                     * a0, * a1;
   const int                  incA = (int)( (unsigned int)(LDA) <<
                                            HPL_LASWP01N_LOG2_DEPTH ),
                              incU = (int)( (unsigned int)(LDU) <<
                                            HPL_LASWP01N_LOG2_DEPTH );
   int                        lda1, nu, nr;
   register int               i, j;
/* ..
 * .. Executable Statements ..
 */
   if( ( M <= 0 ) || ( N <= 0 ) ) return;

   nr = N - ( nu = (int)( ( (unsigned int)(N) >> HPL_LASWP01N_LOG2_DEPTH ) <<
                            HPL_LASWP01N_LOG2_DEPTH ) );

   for( j = 0; j < nu; j += HPL_LASWP01N_DEPTH, A += incA, U += incU )
   {
      for( i = 0; i < M; i++ )
      {
         a0 = A + LINDXA[i];
         if( LINDXAU[i] >= 0 ) { a1 = U + LINDXAU[i]; lda1 = LDU; }
         else                  { a1 = A - LINDXAU[i]; lda1 = LDA; }

         *a1 = *a0; a1 += lda1; a0 += LDA;
#if ( HPL_LASWP01N_DEPTH >  1 )
         *a1 = *a0; a1 += lda1; a0 += LDA;
#endif
#if ( HPL_LASWP01N_DEPTH >  2 )
         *a1 = *a0; a1 += lda1; a0 += LDA; *a1 = *a0; a1 += lda1; a0 += LDA;
#endif
#if ( HPL_LASWP01N_DEPTH >  4 )
         *a1 = *a0; a1 += lda1; a0 += LDA; *a1 = *a0; a1 += lda1; a0 += LDA;
         *a1 = *a0; a1 += lda1; a0 += LDA; *a1 = *a0; a1 += lda1; a0 += LDA;
#endif
#if ( HPL_LASWP01N_DEPTH >  8 )
         *a1 = *a0; a1 += lda1; a0 += LDA; *a1 = *a0; a1 += lda1; a0 += LDA;
         *a1 = *a0; a1 += lda1; a0 += LDA; *a1 = *a0; a1 += lda1; a0 += LDA;
         *a1 = *a0; a1 += lda1; a0 += LDA; *a1 = *a0; a1 += lda1; a0 += LDA;
         *a1 = *a0; a1 += lda1; a0 += LDA; *a1 = *a0; a1 += lda1; a0 += LDA;
#endif
#if ( HPL_LASWP01N_DEPTH > 16 )
         *a1 = *a0; a1 += lda1; a0 += LDA; *a1 = *a0; a1 += lda1; a0 += LDA;
         *a1 = *a0; a1 += lda1; a0 += LDA; *a1 = *a0; a1 += lda1; a0 += LDA;
         *a1 = *a0; a1 += lda1; a0 += LDA; *a1 = *a0; a1 += lda1; a0 += LDA;
         *a1 = *a0; a1 += lda1; a0 += LDA; *a1 = *a0; a1 += lda1; a0 += LDA;
         *a1 = *a0; a1 += lda1; a0 += LDA; *a1 = *a0; a1 += lda1; a0 += LDA;
         *a1 = *a0; a1 += lda1; a0 += LDA; *a1 = *a0; a1 += lda1; a0 += LDA;
         *a1 = *a0; a1 += lda1; a0 += LDA; *a1 = *a0; a1 += lda1; a0 += LDA;
         *a1 = *a0; a1 += lda1; a0 += LDA; *a1 = *a0; a1 += lda1; a0 += LDA;
#endif
      }
   }

   if( nr )
   {
      for( i = 0; i < M; i++ )
      {
         a0 = A + LINDXA[i];
         if( LINDXAU[i] >= 0 ) { a1 = U + LINDXAU[i]; lda1 = LDU; }
         else                  { a1 = A - LINDXAU[i]; lda1 = LDA; }

         for( j = 0; j < nr; j++, a1 += lda1, a0 += LDA ) { *a1 = *a0; }
      }
   }
/*
 * End of HPL_dlaswp01N
 */
} 
