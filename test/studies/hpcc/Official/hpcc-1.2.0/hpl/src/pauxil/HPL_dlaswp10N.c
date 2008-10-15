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
#ifndef HPL_LASWP10N_DEPTH
#define    HPL_LASWP10N_DEPTH       32
#define    HPL_LASWP10N_LOG2_DEPTH   5
#endif

#ifdef HPL_STDC_HEADERS
void HPL_dlaswp10N
(
   const int                        M,
   const int                        N,
   double *                         A,
   const int                        LDA,
   const int *                      IPIV
)
#else
void HPL_dlaswp10N
( M, N, A, LDA, IPIV )
   const int                        M;
   const int                        N;
   double *                         A;
   const int                        LDA;
   const int *                      IPIV;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_dlaswp10N performs a sequence  of  local column interchanges on a
 * matrix A.  One column interchange is initiated  for columns 0 through
 * N-1 of A.
 *
 * Arguments
 * =========
 *
 * M       (local input)                 const int
 *         __arg0__
 *
 * N       (local input)                 const int
 *         On entry,  M  specifies  the number of rows of the array A. M
 *         must be at least zero.
 *
 * A       (local input/output)          double *
 *         On entry, N specifies the number of columns of the array A. N
 *         must be at least zero.
 *
 * LDA     (local input)                 const int
 *         On entry, A  points to an  array of  dimension (LDA,N).  This
 *         array contains the columns onto which the interchanges should
 *         be applied. On exit, A contains the permuted matrix.
 *
 * IPIV    (local input)                 const int *
 *         On entry, LDA specifies the leading dimension of the array A.
 *         LDA must be at least MAX(1,M).
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   double                     r;
   double                     * a0, * a1;
   const int                  incA = ( 1 << HPL_LASWP10N_LOG2_DEPTH );
   int                        jp, mr, mu;
   register int               i, j;
/* ..
 * .. Executable Statements ..
 */
   if( ( M <= 0 ) || ( N <= 0 ) ) return;

   mr = M - ( mu = (int)( ( (unsigned int)(M) >> HPL_LASWP10N_LOG2_DEPTH )
                            << HPL_LASWP10N_LOG2_DEPTH ) );

   for( j = 0; j < N; j++ )
   {
      if( j != ( jp = IPIV[j] ) )
      {
         a0 = A + j * LDA; a1 = A + jp * LDA;

         for( i = 0; i < mu; i += incA, a0 += incA, a1 += incA )
         {
            r = *a0;    *a0    = *a1;    *a1    = r;
#if ( HPL_LASWP10N_DEPTH >  1 )
            r = a0[ 1]; a0[ 1] = a1[ 1]; a1[ 1] = r;
#endif
#if ( HPL_LASWP10N_DEPTH >  2 )
            r = a0[ 2]; a0[ 2] = a1[ 2]; a1[ 2] = r;
            r = a0[ 3]; a0[ 3] = a1[ 3]; a1[ 3] = r;
#endif
#if ( HPL_LASWP10N_DEPTH >  4 )
            r = a0[ 4]; a0[ 4] = a1[ 4]; a1[ 4] = r;
            r = a0[ 5]; a0[ 5] = a1[ 5]; a1[ 5] = r;
            r = a0[ 6]; a0[ 6] = a1[ 6]; a1[ 6] = r;
            r = a0[ 7]; a0[ 7] = a1[ 7]; a1[ 7] = r;
#endif
#if ( HPL_LASWP10N_DEPTH >  8 )
            r = a0[ 8]; a0[ 8] = a1[ 8]; a1[ 8] = r;
            r = a0[ 9]; a0[ 9] = a1[ 9]; a1[ 9] = r;
            r = a0[10]; a0[10] = a1[10]; a1[10] = r;
            r = a0[11]; a0[11] = a1[11]; a1[11] = r;
            r = a0[12]; a0[12] = a1[12]; a1[12] = r;
            r = a0[13]; a0[13] = a1[13]; a1[13] = r;
            r = a0[14]; a0[14] = a1[14]; a1[14] = r;
            r = a0[15]; a0[15] = a1[15]; a1[15] = r;
#endif
#if ( HPL_LASWP10N_DEPTH > 16 )
            r = a0[16]; a0[16] = a1[16]; a1[16] = r;
            r = a0[17]; a0[17] = a1[17]; a1[17] = r;
            r = a0[18]; a0[18] = a1[18]; a1[18] = r;
            r = a0[19]; a0[19] = a1[19]; a1[19] = r;
            r = a0[20]; a0[20] = a1[20]; a1[20] = r;
            r = a0[21]; a0[21] = a1[21]; a1[21] = r;
            r = a0[22]; a0[22] = a1[22]; a1[22] = r;
            r = a0[23]; a0[23] = a1[23]; a1[23] = r;
            r = a0[24]; a0[24] = a1[24]; a1[24] = r;
            r = a0[25]; a0[25] = a1[25]; a1[25] = r;
            r = a0[26]; a0[26] = a1[26]; a1[26] = r;
            r = a0[27]; a0[27] = a1[27]; a1[27] = r;
            r = a0[28]; a0[28] = a1[28]; a1[28] = r;
            r = a0[29]; a0[29] = a1[29]; a1[29] = r;
            r = a0[30]; a0[30] = a1[30]; a1[30] = r;
            r = a0[31]; a0[31] = a1[31]; a1[31] = r;
#endif
         }

         for( i = 0; i < mr; i++ )
         { r = a0[i]; a0[i] = a1[i]; a1[i] = r; }
      }
   }
/*
 * End of HPL_dlaswp10N
 */
}
