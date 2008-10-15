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
#ifndef HPL_LASWP02N_DEPTH
#define    HPL_LASWP02N_DEPTH       32
#define    HPL_LASWP02N_LOG2_DEPTH   5
#endif

#ifdef HPL_STDC_HEADERS
void HPL_dlaswp02N
(
   const int                        M,
   const int                        N,
   const double *                   A,
   const int                        LDA,
   double *                         W0,
   double *                         W,
   const int                        LDW,
   const int *                      LINDXA,
   const int *                      LINDXAU
)
#else
void HPL_dlaswp02N
( M, N, A, LDA, W0, W, LDW, LINDXA, LINDXAU )
   const int                        M;
   const int                        N;
   const double *                   A;
   const int                        LDA;
   double *                         W0;
   double *                         W;
   const int                        LDW;
   const int *                      LINDXA;
   const int *                      LINDXAU;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_dlaswp02N packs scattered rows of an array  A  into workspace  W.
 * The row offsets in A are specified by LINDXA.
 *
 * Arguments
 * =========
 *
 * M       (local input)                 const int
 *         On entry, M  specifies the number of rows of A that should be
 *         copied into W. M must be at least zero.
 *
 * N       (local input)                 const int
 *         On entry, N  specifies the length of rows of A that should be
 *         copied into W. N must be at least zero.
 *
 * A       (local input)                 const double *
 *         On entry, A points to an array of dimension (LDA,N). The rows
 *         of this array specified by LINDXA should be copied into W.
 *
 * LDA     (local input)                 const int
 *         On entry, LDA specifies the leading dimension of the array A.
 *         LDA must be at least MAX(1,M).
 *
 * W0      (local input/output)          double *
 *         On exit,  W0  is  an array of size (M-1)*LDW+1, that contains
 *         the destination offset  in U where the columns of W should be
 *         copied.
 *
 * W       (local output)                double *
 *         On entry, W  is an array of size (LDW,M). On exit, W contains
 *         the  rows LINDXA[i] for i in [0..M) of A stored  contiguously
 *         in W(:,i).
 *
 * LDW     (local input)                 const int
 *         On entry, LDW specifies the leading dimension of the array W.
 *         LDW must be at least MAX(1,N+1).
 *
 * LINDXA  (local input)                 const int *
 *         On entry, LINDXA is an array of dimension M that contains the
 *         local row indexes of A that should be copied into W.
 *
 * LINDXAU (local input)                 const int *
 *         On entry, LINDXAU  is an array of dimension M  that  contains
 *         the local  row indexes of  U that should be copied into A and
 *         replaced by the rows of W.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   const double               * A0 = A, * a0;
   double                     * w0;
   const int                  incA = (int)( (unsigned int)(LDA) <<
                                            HPL_LASWP02N_LOG2_DEPTH );
   int                        nr, nu;
   register int               i, j;
/* ..
 * .. Executable Statements ..
 */
   if( ( M <= 0 ) || ( N <= 0 ) ) return;

   for( i = 0; i < M; i++ ) *(W0+i*LDW) = (double)(LINDXAU[i]);

   nr = N - ( nu = (int)( ( (unsigned int)(N) >> HPL_LASWP02N_LOG2_DEPTH ) <<
                          HPL_LASWP02N_LOG2_DEPTH ) );

   for( j = 0; j < nu;
        j += HPL_LASWP02N_DEPTH, A0 += incA, W += HPL_LASWP02N_DEPTH )
   {
      for( i = 0; i < M; i++ )
      {
         a0 = A0 + LINDXA[i]; w0 = W + i * LDW;

         w0[ 0] = *a0; a0 += LDA;
#if ( HPL_LASWP02N_DEPTH >  1 )
         w0[ 1] = *a0; a0 += LDA;
#endif
#if ( HPL_LASWP02N_DEPTH >  2 )
         w0[ 2] = *a0; a0 += LDA; w0[ 3] = *a0; a0 += LDA;
#endif
#if ( HPL_LASWP02N_DEPTH >  4 )
         w0[ 4] = *a0; a0 += LDA; w0[ 5] = *a0; a0 += LDA;
         w0[ 6] = *a0; a0 += LDA; w0[ 7] = *a0; a0 += LDA;
#endif
#if ( HPL_LASWP02N_DEPTH >  8 )
         w0[ 8] = *a0; a0 += LDA; w0[ 9] = *a0; a0 += LDA;
         w0[10] = *a0; a0 += LDA; w0[11] = *a0; a0 += LDA;
         w0[12] = *a0; a0 += LDA; w0[13] = *a0; a0 += LDA;
         w0[14] = *a0; a0 += LDA; w0[15] = *a0; a0 += LDA;
#endif
#if ( HPL_LASWP02N_DEPTH > 16 )
         w0[16] = *a0; a0 += LDA; w0[17] = *a0; a0 += LDA;
         w0[18] = *a0; a0 += LDA; w0[19] = *a0; a0 += LDA;
         w0[20] = *a0; a0 += LDA; w0[21] = *a0; a0 += LDA;
         w0[22] = *a0; a0 += LDA; w0[23] = *a0; a0 += LDA;
         w0[24] = *a0; a0 += LDA; w0[25] = *a0; a0 += LDA;
         w0[26] = *a0; a0 += LDA; w0[27] = *a0; a0 += LDA;
         w0[28] = *a0; a0 += LDA; w0[29] = *a0; a0 += LDA;
         w0[30] = *a0; a0 += LDA; w0[31] = *a0; a0 += LDA;
#endif
      }
   }

   if( nr > 0 )
   {
      for( i = 0; i < M; i++ )
      {
         a0 = A0 + LINDXA[i]; w0 = W + i * LDW;
         for( j = 0; j < nr; j++, a0 += LDA ) { w0[j] = *a0; }
      }
   }
/*
 * End of HPL_dlaswp02N
 */
} 
