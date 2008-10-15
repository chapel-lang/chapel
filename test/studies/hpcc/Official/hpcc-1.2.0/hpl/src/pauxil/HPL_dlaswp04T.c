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
#ifndef HPL_LASWP04T_DEPTH
#define    HPL_LASWP04T_DEPTH       32
#define    HPL_LASWP04T_LOG2_DEPTH   5
#endif

#ifdef HPL_STDC_HEADERS
void HPL_dlaswp04T
(
   const int                        M0,
   const int                        M1,
   const int                        N,
   double *                         U,
   const int                        LDU,
   double *                         A,
   const int                        LDA,
   const double *                   W0,
   const double *                   W,
   const int                        LDW,
   const int *                      LINDXA,
   const int *                      LINDXAU
)
#else
void HPL_dlaswp04T
( M0, M1, N, U, LDU, A, LDA, W0, W, LDW, LINDXA, LINDXAU )
   const int                        M0;
   const int                        M1;
   const int                        N;
   double *                         U;
   const int                        LDU;
   double *                         A;
   const int                        LDA;
   const double *                   W0;
   const double *                   W;
   const int                        LDW;
   const int *                      LINDXA;
   const int *                      LINDXAU;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_dlaswp04T copies M0 columns of U into rows of A and replaces those
 * columns of U with columns of W. In addition M1 - M0 columns of W  are
 * copied into U.
 *
 * Arguments
 * =========
 *
 * M0      (local input)                 const int
 *         On entry, M0 specifies the number of columns of U that should
 *         be copied into A and replaced by columns of W.  M0 must be at
 *         least zero.
 *
 * M1      (local input)                 const int
 *         On entry, M1 specifies  the number of columnns of W that will
 *         be copied into U. M1 must be at least zero.
 *
 * N       (local input)                 const int
 *         On entry,  N  specifies the length of the columns of  U  that
 *         will be copied into rows of A. N must be at least zero.
 *
 * U       (local input/output)          double *
 *         On entry,  U  points  to an array of dimension (LDU,*).  This
 *         array contains the columns that are to be copied into rows of
 *         A.
 *
 * LDU     (local input)                 const int
 *         On entry, LDU specifies the leading dimension of the array U.
 *         LDU must be at least MAX(1,N).
 *
 * A       (local output)                double *
 *         On entry, A points to an array of dimension (LDA,N). On exit,
 *         the  rows of this array specified by  LINDXA  are replaced by
 *         columns of U indicated by LINDXAU.
 *
 * LDA     (local input)                 const int
 *         On entry, LDA specifies the leading dimension of the array A.
 *         LDA must be at least MAX(1,M0).
 *
 * W0      (local input)                 const double *
 *         On entry,  W0  is an array of size (M-1)*LDW+1, that contains
 *         the destination offset  in U where the columns of W should be
 *         copied.
 *
 * W       (local input)                 const double *
 *         On entry, W  is an array of size (LDW,M0+M1),  that  contains
 *         data to be copied into U.  For i in [M0..M0+M1),  the entries
 *         W(:,i) are copied into the column W0(i*LDW) of U.
 *
 * LDW     (local input)                 const int
 *         On entry, LDW specifies the leading dimension of the array W.
 *         LDW must be at least MAX(1,N+1).
 *
 * LINDXA  (local input)                 const int *
 *         On entry, LINDXA  is an array of dimension  M0 containing the
 *         local row indexes A into which columns of U are copied.
 *
 * LINDXAU (local input)                 const int *
 *         On entry, LINDXAU  is an array of dimension M0 that  contains
 *         the  local column indexes of  U  that should be copied into A
 *         and replaced by the columns of W.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   const double               * w = W, * w0;
   double                     * a0, * u0;
   const int                  incA = (int)( (unsigned int)(LDA) <<
                                            HPL_LASWP04T_LOG2_DEPTH ),
                              incU = (   1 << HPL_LASWP04T_LOG2_DEPTH );
   int                        nr, nu;
   register int               i, j;
/* ..
 * .. Executable Statements ..
 */
   if( ( ( M0 <= 0 ) && ( M1 <= 0 ) ) || ( N <= 0 ) ) return;

   nr = N - ( nu = (int)( ( (unsigned int)(N) >> HPL_LASWP04T_LOG2_DEPTH ) <<
                          HPL_LASWP04T_LOG2_DEPTH ) );

   for( j = 0; j < nu; j += HPL_LASWP04T_DEPTH, A += incA, U += incU,
        w += HPL_LASWP04T_DEPTH )
   {
      for( i = 0; i < M0; i++ )
      {
         a0 = A + LINDXA[i]; u0 = U + LINDXAU[i] * LDU; w0 = w + i * LDW;

         *a0 = u0[ 0]; u0[ 0] = w0[ 0]; a0 += LDA;
#if ( HPL_LASWP04T_DEPTH >  1 )
         *a0 = u0[ 1]; u0[ 1] = w0[ 1]; a0 += LDA;
#endif
#if ( HPL_LASWP04T_DEPTH >  2 )
         *a0 = u0[ 2]; u0[ 2] = w0[ 2]; a0 += LDA;
         *a0 = u0[ 3]; u0[ 3] = w0[ 3]; a0 += LDA;
#endif
#if ( HPL_LASWP04T_DEPTH >  4 )
         *a0 = u0[ 4]; u0[ 4] = w0[ 4]; a0 += LDA;
         *a0 = u0[ 5]; u0[ 5] = w0[ 5]; a0 += LDA;
         *a0 = u0[ 6]; u0[ 6] = w0[ 6]; a0 += LDA;
         *a0 = u0[ 7]; u0[ 7] = w0[ 7]; a0 += LDA;
#endif
#if ( HPL_LASWP04T_DEPTH >  8 )
         *a0 = u0[ 8]; u0[ 8] = w0[ 8]; a0 += LDA;
         *a0 = u0[ 9]; u0[ 9] = w0[ 9]; a0 += LDA;
         *a0 = u0[10]; u0[10] = w0[10]; a0 += LDA;
         *a0 = u0[11]; u0[11] = w0[11]; a0 += LDA;
         *a0 = u0[12]; u0[12] = w0[12]; a0 += LDA;
         *a0 = u0[13]; u0[13] = w0[13]; a0 += LDA;
         *a0 = u0[14]; u0[14] = w0[14]; a0 += LDA;
         *a0 = u0[15]; u0[15] = w0[15]; a0 += LDA;
#endif
#if ( HPL_LASWP04T_DEPTH > 16 )
         *a0 = u0[16]; u0[16] = w0[16]; a0 += LDA;
         *a0 = u0[17]; u0[17] = w0[17]; a0 += LDA;
         *a0 = u0[18]; u0[18] = w0[18]; a0 += LDA;
         *a0 = u0[19]; u0[19] = w0[19]; a0 += LDA;
         *a0 = u0[20]; u0[20] = w0[20]; a0 += LDA;
         *a0 = u0[21]; u0[21] = w0[21]; a0 += LDA;
         *a0 = u0[22]; u0[22] = w0[22]; a0 += LDA;
         *a0 = u0[23]; u0[23] = w0[23]; a0 += LDA;
         *a0 = u0[24]; u0[24] = w0[24]; a0 += LDA;
         *a0 = u0[25]; u0[25] = w0[25]; a0 += LDA;
         *a0 = u0[26]; u0[26] = w0[26]; a0 += LDA;
         *a0 = u0[27]; u0[27] = w0[27]; a0 += LDA;
         *a0 = u0[28]; u0[28] = w0[28]; a0 += LDA;
         *a0 = u0[29]; u0[29] = w0[29]; a0 += LDA;
         *a0 = u0[30]; u0[30] = w0[30]; a0 += LDA;
         *a0 = u0[31]; u0[31] = w0[31]; a0 += LDA;
#endif
      }
      for( i = M0; i < M1; i++ )
      {
         u0 = U + (int)(*(W0+i*LDW)) * LDU; w0 = w + i * LDW;

         u0[ 0] = w0[ 0];
#if ( HPL_LASWP04T_DEPTH >  1 )
         u0[ 1] = w0[ 1];
#endif
#if ( HPL_LASWP04T_DEPTH >  2 )
         u0[ 2] = w0[ 2]; u0[ 3] = w0[ 3];
#endif
#if ( HPL_LASWP04T_DEPTH >  4 )
         u0[ 4] = w0[ 4]; u0[ 5] = w0[ 5]; u0[ 6] = w0[ 6]; u0[ 7] = w0[ 7];
#endif
#if ( HPL_LASWP04T_DEPTH >  8 )
         u0[ 8] = w0[ 8]; u0[ 9] = w0[ 9]; u0[10] = w0[10]; u0[11] = w0[11];
         u0[12] = w0[12]; u0[13] = w0[13]; u0[14] = w0[14]; u0[15] = w0[15];
#endif
#if ( HPL_LASWP04T_DEPTH > 16 )
         u0[16] = w0[16]; u0[17] = w0[17]; u0[18] = w0[18]; u0[19] = w0[19];
         u0[20] = w0[20]; u0[21] = w0[21]; u0[22] = w0[22]; u0[23] = w0[23];
         u0[24] = w0[24]; u0[25] = w0[25]; u0[26] = w0[26]; u0[27] = w0[27];
         u0[28] = w0[28]; u0[29] = w0[29]; u0[30] = w0[30]; u0[31] = w0[31];
#endif
      }
   }

   if( nr > 0 )
   {
      for( i = 0; i < M0; i++ )
      {
         a0 = A + LINDXA[i]; u0 = U + LINDXAU[i] * LDU; w0 = w + i * LDW;
         for( j = 0; j < nr; j++, a0 += LDA ) { *a0 = u0[j]; u0[j] = w0[j]; }
      }
      for( i = M0; i < M1; i++ )
      {
         u0 = U + (int)(*(W0+i*LDW)) * LDU; w0 = w + i * LDW;
         for( j = 0; j < nr; j++ ) { u0[j] = w0[j]; }
      }
   }
/*
 * End of HPL_dlaswp04T
 */
} 
