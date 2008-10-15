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
void HPL_dmatgen
(
   const int                        M,
   const int                        N,
   double *                         A,
   const int                        LDA,
   const int                        ISEED
)
#else
void HPL_dmatgen
( M, N, A, LDA, ISEED )
   const int                        M;
   const int                        N;
   double *                         A;
   const int                        LDA;
   const int                        ISEED;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_dmatgen generates (or regenerates) a random matrix A.
 *  
 * The  pseudo-random  generator uses the linear congruential algorithm:
 * X(n+1) = (a * X(n) + c) mod m  as  described  in the  Art of Computer
 * Programming, Knuth 1973, Vol. 2.
 *
 * Arguments
 * =========
 *
 * M       (input)                       const int
 *         On entry,  M  specifies  the number  of rows of the matrix A.
 *         M must be at least zero.
 *
 * N       (input)                       const int
 *         On entry,  N specifies the number of columns of the matrix A.
 *         N must be at least zero.
 *
 * A       (output)                      double *
 *         On entry, A points to an array of dimension (LDA,N). On exit,
 *         this  array  contains   the   coefficients  of  the  randomly
 *         generated matrix.
 *
 * LDA     (input)                       const int
 *         On entry, LDA specifies the leading dimension of the array A.
 *         LDA must be at least max(1,M).
 *
 * ISEED   (input)                       const int
 *         On entry, ISEED  specifies  the  seed  number to generate the
 *         matrix A. ISEED must be at least zero.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   int                        iadd[2], ia1[2], ic1[2], iran1[2],
                              jseed[2], mult[2];
   int                        i, incA = LDA - M, j;
/* ..
 * .. Executable Statements ..
 */
   if( ( M <= 0 ) || ( N <= 0 ) ) return;
/*
 * Initialize the random sequence
 */
   mult [0] = HPL_MULT0; mult [1] = HPL_MULT1;
   iadd [0] = HPL_IADD0; iadd [1] = HPL_IADD1;
   jseed[0] = ISEED;     jseed[1] = 0;

   HPL_xjumpm( 1, mult, iadd, jseed, iran1, ia1, ic1 );
   HPL_setran( 0, iran1 ); HPL_setran( 1, ia1 ); HPL_setran( 2, ic1 );
/*
 * Generate an M by N matrix
 */
   for( j = 0; j < N; A += incA, j++ )
      for( i = 0; i < M; A++, i++ ) *A = HPL_rand();
/*
 * End of HPL_dmatgen
 */
}
