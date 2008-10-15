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
 * Define default value for unrolling factors
 * #ifndef HPL_LATCPY_M_DEPTH
 * #define    HPL_LATCPY_M_DEPTH      32
 * #define    HPL_LATCPY_LOG2_M_DEPTH  5
 * #endif
 * #ifndef HPL_LATCPY_N_DEPTH
 * #define    HPL_LATCPY_N_DEPTH       4
 * #define    HPL_LATCPY_LOG2_N_DEPTH  2
 * #endif
 */
#ifndef HPL_LATCPY_M_DEPTH
#define    HPL_LATCPY_M_DEPTH       4
#define    HPL_LATCPY_LOG2_M_DEPTH  2
#endif
#ifndef HPL_LATCPY_N_DEPTH
#define    HPL_LATCPY_N_DEPTH       2
#define    HPL_LATCPY_LOG2_N_DEPTH  1
#endif

#ifdef HPL_STDC_HEADERS
void HPL_dlatcpy
(
   const int                        M,
   const int                        N,
   const double *                   A,
   const int                        LDA,
   double *                         B,
   const int                        LDB
)
#else
void HPL_dlatcpy
( M, N, A, LDA, B, LDB )
   const int                        M;
   const int                        N;
   const double *                   A;
   const int                        LDA;
   double *                         B;
   const int                        LDB;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_dlatcpy copies the transpose of an array A into an array B.
 * 
 *
 * Arguments
 * =========
 *
 * M       (local input)                 const int
 *         On entry,  M specifies the number of  rows of the array B and
 *         the number of columns of A. M must be at least zero.
 *
 * N       (local input)                 const int
 *         On entry,  N specifies the number of  rows of the array A and
 *         the number of columns of B. N must be at least zero.
 *
 * A       (local input)                 const double *
 *         On entry, A points to an array of dimension (LDA,M).
 *
 * LDA     (local input)                 const int
 *         On entry, LDA specifies the leading dimension of the array A.
 *         LDA must be at least MAX(1,N).
 *
 * B       (local output)                double *
 *         On entry, B points to an array of dimension (LDB,N). On exit,
 *         B is overwritten with the transpose of A.
 *
 * LDB     (local input)                 const int
 *         On entry, LDB specifies the leading dimension of the array B.
 *         LDB must be at least MAX(1,M).
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
#ifdef HPL_LATCPY_USE_COPY
   register int               j;
#else
#if   ( HPL_LATCPY_N_DEPTH == 1 )
   const double               * A0 = A;
   double                     * B0 = B;
#elif ( HPL_LATCPY_N_DEPTH == 2 )
   const double               * A0 = A,              * A1 = A + 1;
   double                     * B0 = B,              * B1 = B +     LDB;
#elif ( HPL_LATCPY_N_DEPTH == 4 )
   const double               * A0 = A,              * A1 = A + 1,
                              * A2 = A + 2,          * A3 = A + 3;
   double                     * B0 = B,              * B1 = B +     LDB,
                              * B2 = B + (LDB << 1), * B3 = B + 3 * LDB;
#endif
   const int                  incA = -M * LDA + (1 << HPL_LATCPY_LOG2_N_DEPTH),
                              incB = ( (unsigned int)(LDB) <<
                                       HPL_LATCPY_LOG2_N_DEPTH ) - M,
                              incA0 = -M * LDA + 1, incB0 = LDB - M;
   int                        mu, nu;
   register int               i, j;
#endif
/* ..
 * .. Executable Statements ..
 */
   if( ( M <= 0 ) || ( N <= 0 ) ) return;

#ifdef HPL_LATCPY_USE_COPY
   for( j = 0; j < N; j++, B0 += LDB ) HPL_dcopy( M, A0+j, LDA, B0, 1 );
#else
   mu = (int)( ( (unsigned int)(M) >> HPL_LATCPY_LOG2_M_DEPTH ) <<
                                      HPL_LATCPY_LOG2_M_DEPTH );
   nu = (int)( ( (unsigned int)(N) >> HPL_LATCPY_LOG2_N_DEPTH ) <<
                                      HPL_LATCPY_LOG2_N_DEPTH );

   for( j = 0; j < nu; j += HPL_LATCPY_N_DEPTH )
   {
      for( i = 0; i < mu; i += HPL_LATCPY_M_DEPTH )
      {
#if   ( HPL_LATCPY_N_DEPTH == 1 )
         B0[ 0] = *A0; A0 += LDA;
#elif ( HPL_LATCPY_N_DEPTH == 2 )
         B0[ 0] = *A0; A0 += LDA; B1[ 0] = *A1; A1 += LDA;
#elif ( HPL_LATCPY_N_DEPTH == 4 )
         B0[ 0] = *A0; A0 += LDA; B1[ 0] = *A1; A1 += LDA;
         B2[ 0] = *A2; A2 += LDA; B3[ 0] = *A3; A3 += LDA;
#endif

#if ( HPL_LATCPY_M_DEPTH >  1 )

#if   ( HPL_LATCPY_N_DEPTH == 1 )
         B0[ 1] = *A0; A0 += LDA;
#elif ( HPL_LATCPY_N_DEPTH == 2 )
         B0[ 1] = *A0; A0 += LDA; B1[ 1] = *A1; A1 += LDA;
#elif ( HPL_LATCPY_N_DEPTH == 4 )
         B0[ 1] = *A0; A0 += LDA; B1[ 1] = *A1; A1 += LDA;
         B2[ 1] = *A2; A2 += LDA; B3[ 1] = *A3; A3 += LDA;
#endif

#endif
#if ( HPL_LATCPY_M_DEPTH >  2 )

#if   ( HPL_LATCPY_N_DEPTH == 1 )
         B0[ 2] = *A0; A0 += LDA; B0[ 3] = *A0; A0 += LDA;
#elif ( HPL_LATCPY_N_DEPTH == 2 )
         B0[ 2] = *A0; A0 += LDA; B1[ 2] = *A1; A1 += LDA;
         B0[ 3] = *A0; A0 += LDA; B1[ 3] = *A1; A1 += LDA;
#elif ( HPL_LATCPY_N_DEPTH == 4 )
         B0[ 2] = *A0; A0 += LDA; B1[ 2] = *A1; A1 += LDA;
         B2[ 2] = *A2; A2 += LDA; B3[ 2] = *A3; A3 += LDA;
         B0[ 3] = *A0; A0 += LDA; B1[ 3] = *A1; A1 += LDA;
         B2[ 3] = *A2; A2 += LDA; B3[ 3] = *A3; A3 += LDA;
#endif

#endif
#if ( HPL_LATCPY_M_DEPTH >  4 )

#if   ( HPL_LATCPY_N_DEPTH == 1 )
         B0[ 4] = *A0; A0 += LDA; B0[ 5] = *A0; A0 += LDA;
         B0[ 6] = *A0; A0 += LDA; B0[ 7] = *A0; A0 += LDA;
#elif ( HPL_LATCPY_N_DEPTH == 2 )
         B0[ 4] = *A0; A0 += LDA; B1[ 4] = *A1; A1 += LDA;
         B0[ 5] = *A0; A0 += LDA; B1[ 5] = *A1; A1 += LDA;
         B0[ 6] = *A0; A0 += LDA; B1[ 6] = *A1; A1 += LDA;
         B0[ 7] = *A0; A0 += LDA; B1[ 7] = *A1; A1 += LDA;
#elif ( HPL_LATCPY_N_DEPTH == 4 )
         B0[ 4] = *A0; A0 += LDA; B1[ 4] = *A1; A1 += LDA;
         B2[ 4] = *A2; A2 += LDA; B3[ 4] = *A3; A3 += LDA;
         B0[ 5] = *A0; A0 += LDA; B1[ 5] = *A1; A1 += LDA;
         B2[ 5] = *A2; A2 += LDA; B3[ 5] = *A3; A3 += LDA;
         B0[ 6] = *A0; A0 += LDA; B1[ 6] = *A1; A1 += LDA;
         B2[ 6] = *A2; A2 += LDA; B3[ 6] = *A3; A3 += LDA;
         B0[ 7] = *A0; A0 += LDA; B1[ 7] = *A1; A1 += LDA;
         B2[ 7] = *A2; A2 += LDA; B3[ 7] = *A3; A3 += LDA;
#endif

#endif
#if ( HPL_LATCPY_M_DEPTH >  8 )

#if   ( HPL_LATCPY_N_DEPTH == 1 )
         B0[ 8] = *A0; A0 += LDA; B0[ 9] = *A0; A0 += LDA;
         B0[10] = *A0; A0 += LDA; B0[11] = *A0; A0 += LDA;
         B0[12] = *A0; A0 += LDA; B0[13] = *A0; A0 += LDA;
         B0[14] = *A0; A0 += LDA; B0[15] = *A0; A0 += LDA;
#elif ( HPL_LATCPY_N_DEPTH == 2 )
         B0[ 8] = *A0; A0 += LDA; B1[ 8] = *A1; A1 += LDA;
         B0[ 9] = *A0; A0 += LDA; B1[ 9] = *A1; A1 += LDA;
         B0[10] = *A0; A0 += LDA; B1[10] = *A1; A1 += LDA;
         B0[11] = *A0; A0 += LDA; B1[11] = *A1; A1 += LDA;
         B0[12] = *A0; A0 += LDA; B1[12] = *A1; A1 += LDA;
         B0[13] = *A0; A0 += LDA; B1[13] = *A1; A1 += LDA;
         B0[14] = *A0; A0 += LDA; B1[14] = *A1; A1 += LDA;
         B0[15] = *A0; A0 += LDA; B1[15] = *A1; A1 += LDA;
#elif ( HPL_LATCPY_N_DEPTH == 4 )
         B0[ 8] = *A0; A0 += LDA; B1[ 8] = *A1; A1 += LDA;
         B2[ 8] = *A2; A2 += LDA; B3[ 8] = *A3; A3 += LDA;
         B0[ 9] = *A0; A0 += LDA; B1[ 9] = *A1; A1 += LDA;
         B2[ 9] = *A2; A2 += LDA; B3[ 9] = *A3; A3 += LDA;
         B0[10] = *A0; A0 += LDA; B1[10] = *A1; A1 += LDA;
         B2[10] = *A2; A2 += LDA; B3[10] = *A3; A3 += LDA;
         B0[11] = *A0; A0 += LDA; B1[11] = *A1; A1 += LDA;
         B2[11] = *A2; A2 += LDA; B3[11] = *A3; A3 += LDA;
         B0[12] = *A0; A0 += LDA; B1[12] = *A1; A1 += LDA;
         B2[12] = *A2; A2 += LDA; B3[12] = *A3; A3 += LDA;
         B0[13] = *A0; A0 += LDA; B1[13] = *A1; A1 += LDA;
         B2[13] = *A2; A2 += LDA; B3[13] = *A3; A3 += LDA;
         B0[14] = *A0; A0 += LDA; B1[14] = *A1; A1 += LDA;
         B2[14] = *A2; A2 += LDA; B3[14] = *A3; A3 += LDA;
         B0[15] = *A0; A0 += LDA; B1[15] = *A1; A1 += LDA;
         B2[15] = *A2; A2 += LDA; B3[15] = *A3; A3 += LDA;
#endif

#endif
#if ( HPL_LATCPY_M_DEPTH > 16 )

#if   ( HPL_LATCPY_N_DEPTH == 1 )
         B0[16] = *A0; A0 += LDA; B0[17] = *A0; A0 += LDA;
         B0[18] = *A0; A0 += LDA; B0[19] = *A0; A0 += LDA;
         B0[20] = *A0; A0 += LDA; B0[21] = *A0; A0 += LDA;
         B0[22] = *A0; A0 += LDA; B0[23] = *A0; A0 += LDA;
         B0[24] = *A0; A0 += LDA; B0[25] = *A0; A0 += LDA;
         B0[26] = *A0; A0 += LDA; B0[27] = *A0; A0 += LDA;
         B0[28] = *A0; A0 += LDA; B0[29] = *A0; A0 += LDA;
         B0[30] = *A0; A0 += LDA; B0[31] = *A0; A0 += LDA;
#elif ( HPL_LATCPY_N_DEPTH == 2 )
         B0[16] = *A0; A0 += LDA; B1[16] = *A1; A1 += LDA;
         B0[17] = *A0; A0 += LDA; B1[17] = *A1; A1 += LDA;
         B0[18] = *A0; A0 += LDA; B1[18] = *A1; A1 += LDA;
         B0[19] = *A0; A0 += LDA; B1[19] = *A1; A1 += LDA;
         B0[20] = *A0; A0 += LDA; B1[20] = *A1; A1 += LDA;
         B0[21] = *A0; A0 += LDA; B1[21] = *A1; A1 += LDA;
         B0[22] = *A0; A0 += LDA; B1[22] = *A1; A1 += LDA;
         B0[23] = *A0; A0 += LDA; B1[23] = *A1; A1 += LDA;
         B0[24] = *A0; A0 += LDA; B1[24] = *A1; A1 += LDA;
         B0[25] = *A0; A0 += LDA; B1[25] = *A1; A1 += LDA;
         B0[26] = *A0; A0 += LDA; B1[26] = *A1; A1 += LDA;
         B0[27] = *A0; A0 += LDA; B1[27] = *A1; A1 += LDA;
         B0[28] = *A0; A0 += LDA; B1[28] = *A1; A1 += LDA;
         B0[29] = *A0; A0 += LDA; B1[29] = *A1; A1 += LDA;
         B0[30] = *A0; A0 += LDA; B1[30] = *A1; A1 += LDA;
         B0[31] = *A0; A0 += LDA; B1[31] = *A1; A1 += LDA;
#elif ( HPL_LATCPY_N_DEPTH == 4 )
         B0[16] = *A0; A0 += LDA; B1[16] = *A1; A1 += LDA;
         B2[16] = *A2; A2 += LDA; B3[16] = *A3; A3 += LDA;
         B0[17] = *A0; A0 += LDA; B1[17] = *A1; A1 += LDA;
         B2[17] = *A2; A2 += LDA; B3[17] = *A3; A3 += LDA;
         B0[18] = *A0; A0 += LDA; B1[18] = *A1; A1 += LDA;
         B2[18] = *A2; A2 += LDA; B3[18] = *A3; A3 += LDA;
         B0[19] = *A0; A0 += LDA; B1[19] = *A1; A1 += LDA;
         B2[19] = *A2; A2 += LDA; B3[19] = *A3; A3 += LDA;
         B0[20] = *A0; A0 += LDA; B1[20] = *A1; A1 += LDA;
         B2[20] = *A2; A2 += LDA; B3[20] = *A3; A3 += LDA;
         B0[21] = *A0; A0 += LDA; B1[21] = *A1; A1 += LDA;
         B2[21] = *A2; A2 += LDA; B3[21] = *A3; A3 += LDA;
         B0[22] = *A0; A0 += LDA; B1[22] = *A1; A1 += LDA;
         B2[22] = *A2; A2 += LDA; B3[22] = *A3; A3 += LDA;
         B0[23] = *A0; A0 += LDA; B1[23] = *A1; A1 += LDA;
         B2[23] = *A2; A2 += LDA; B3[23] = *A3; A3 += LDA;
         B0[24] = *A0; A0 += LDA; B1[24] = *A1; A1 += LDA;
         B2[24] = *A2; A2 += LDA; B3[24] = *A3; A3 += LDA;
         B0[25] = *A0; A0 += LDA; B1[25] = *A1; A1 += LDA;
         B2[25] = *A2; A2 += LDA; B3[25] = *A3; A3 += LDA;
         B0[26] = *A0; A0 += LDA; B1[26] = *A1; A1 += LDA;
         B2[26] = *A2; A2 += LDA; B3[26] = *A3; A3 += LDA;
         B0[27] = *A0; A0 += LDA; B1[27] = *A1; A1 += LDA;
         B2[27] = *A2; A2 += LDA; B3[27] = *A3; A3 += LDA;
         B0[28] = *A0; A0 += LDA; B1[28] = *A1; A1 += LDA;
         B2[28] = *A2; A2 += LDA; B3[28] = *A3; A3 += LDA;
         B0[29] = *A0; A0 += LDA; B1[29] = *A1; A1 += LDA;
         B2[29] = *A2; A2 += LDA; B3[29] = *A3; A3 += LDA;
         B0[30] = *A0; A0 += LDA; B1[30] = *A1; A1 += LDA;
         B2[30] = *A2; A2 += LDA; B3[30] = *A3; A3 += LDA;
         B0[31] = *A0; A0 += LDA; B1[31] = *A1; A1 += LDA;
         B2[31] = *A2; A2 += LDA; B3[31] = *A3; A3 += LDA;
#endif

#endif
#if   ( HPL_LATCPY_N_DEPTH == 1 )
         B0 += HPL_LATCPY_M_DEPTH;
#elif ( HPL_LATCPY_N_DEPTH == 2 )
         B0 += HPL_LATCPY_M_DEPTH; B1 += HPL_LATCPY_M_DEPTH;
#elif ( HPL_LATCPY_N_DEPTH == 4 )
         B0 += HPL_LATCPY_M_DEPTH; B1 += HPL_LATCPY_M_DEPTH;
         B2 += HPL_LATCPY_M_DEPTH; B3 += HPL_LATCPY_M_DEPTH;
#endif
      }

      for( i = mu; i < M; i++ )
      {
#if   ( HPL_LATCPY_N_DEPTH == 1 )
         *B0 = *A0; B0++; A0 += LDA;
#elif ( HPL_LATCPY_N_DEPTH == 2 )
         *B0 = *A0; B0++; A0 += LDA; *B1 = *A1; B1++; A1 += LDA;
#elif ( HPL_LATCPY_N_DEPTH == 4 )
         *B0 = *A0; B0++; A0 += LDA; *B1 = *A1; B1++; A1 += LDA;
         *B2 = *A2; B2++; A2 += LDA; *B3 = *A3; B3++; A3 += LDA;
#endif
      }

#if   ( HPL_LATCPY_N_DEPTH == 1 )
      A0 += incA; B0 += incB;
#elif ( HPL_LATCPY_N_DEPTH == 2 )
      A0 += incA; A1 += incA; B0 += incB; B1 += incB;
#elif ( HPL_LATCPY_N_DEPTH == 4 )
      A0 += incA; A1 += incA; A2 += incA; A3 += incA;
      B0 += incB; B1 += incB; B2 += incB; B3 += incB;
#endif
   }

   for( j = nu; j < N; j++, B0 += incB0, A0 += incA0 )
   {
      for( i = 0; i < mu; i += HPL_LATCPY_M_DEPTH, B0 += HPL_LATCPY_M_DEPTH )
      {
         B0[ 0]=*A0; A0 += LDA;
#if ( HPL_LATCPY_M_DEPTH >  1 )
         B0[ 1]=*A0; A0 += LDA;
#endif
#if ( HPL_LATCPY_M_DEPTH >  2 )
         B0[ 2]=*A0; A0 += LDA; B0[ 3]=*A0; A0 += LDA;
#endif
#if ( HPL_LATCPY_M_DEPTH >  4 )
         B0[ 4]=*A0; A0 += LDA; B0[ 5]=*A0; A0 += LDA;
         B0[ 6]=*A0; A0 += LDA; B0[ 7]=*A0; A0 += LDA;
#endif
#if ( HPL_LATCPY_M_DEPTH >  8 )
         B0[ 8]=*A0; A0 += LDA; B0[ 9]=*A0; A0 += LDA;
         B0[10]=*A0; A0 += LDA; B0[11]=*A0; A0 += LDA;
         B0[12]=*A0; A0 += LDA; B0[13]=*A0; A0 += LDA;
         B0[14]=*A0; A0 += LDA; B0[15]=*A0; A0 += LDA;
#endif
#if ( HPL_LATCPY_M_DEPTH > 16 )
         B0[16]=*A0; A0 += LDA; B0[17]=*A0; A0 += LDA;
         B0[18]=*A0; A0 += LDA; B0[19]=*A0; A0 += LDA;
         B0[20]=*A0; A0 += LDA; B0[21]=*A0; A0 += LDA;
         B0[22]=*A0; A0 += LDA; B0[23]=*A0; A0 += LDA;
         B0[24]=*A0; A0 += LDA; B0[25]=*A0; A0 += LDA;
         B0[26]=*A0; A0 += LDA; B0[27]=*A0; A0 += LDA;
         B0[28]=*A0; A0 += LDA; B0[29]=*A0; A0 += LDA;
         B0[30]=*A0; A0 += LDA; B0[31]=*A0; A0 += LDA;
#endif
      }

      for( i = mu; i < M; i++, B0++, A0 += LDA ) { *B0 = *A0; }
   }
#endif
/*
 * End of HPL_dlatcpy
 */
} 
