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
void HPL_xjumpm
(
   const int                        JUMPM,
   int *                            MULT,
   int *                            IADD,
   int *                            IRANN,
   int *                            IRANM,
   int *                            IAM,
   int *                            ICM
)
#else
void HPL_xjumpm
( JUMPM, MULT, IADD, IRANN, IRANM, IAM, ICM )
   const int                        JUMPM;
   int *                            MULT;
   int *                            IADD;
   int *                            IRANN;
   int *                            IRANM;
   int *                            IAM;
   int *                            ICM;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_xjumpm computes  the constants  A and C  to jump JUMPM numbers in
 * the random sequence: X(n+JUMPM) = A*X(n)+C.  The constants encoded in
 * MULT and IADD  specify  how to jump from one entry in the sequence to
 * the next.
 *
 * Arguments
 * =========
 *
 * JUMPM   (local input)                 const int
 *         On entry,  JUMPM  specifies  the  number  of entries  in  the
 *         sequence to jump over. When JUMPM is less or equal than zero,
 *         A and C are not computed, IRANM is set to IRANN corresponding
 *         to a jump of size zero.
 *
 * MULT    (local input)                 int *
 *         On entry, MULT is an array of dimension 2,  that contains the
 *         16-lower  and 15-higher bits of the constant  a  to jump from
 *         X(n) to X(n+1) = a*X(n) + c in the random sequence.
 *
 * IADD    (local input)                 int *
 *         On entry, IADD is an array of dimension 2,  that contains the
 *         16-lower  and 15-higher bits of the constant  c  to jump from
 *         X(n) to X(n+1) = a*X(n) + c in the random sequence.
 *
 * IRANN   (local input)                 int *
 *         On entry, IRANN is an array of dimension 2. that contains the
 *         16-lower and 15-higher bits of the encoding of X(n).
 *
 * IRANM   (local output)                int *
 *         On entry,  IRANM  is an array of dimension 2.   On exit, this
 *         array  contains respectively  the 16-lower and 15-higher bits
 *         of the encoding of X(n+JUMPM).
 *
 * IAM     (local output)                int *
 *         On entry, IAM is an array of dimension 2. On exit, when JUMPM
 *         is  greater  than  zero,  this  array  contains  the  encoded
 *         constant  A  to jump from  X(n) to  X(n+JUMPM)  in the random
 *         sequence. IAM(0:1)  contains  respectively  the  16-lower and
 *         15-higher  bits  of this constant  A. When  JUMPM  is less or
 *         equal than zero, this array is not referenced.
 *
 * ICM     (local output)                int *
 *         On entry, ICM is an array of dimension 2. On exit, when JUMPM
 *         is  greater  than  zero,  this  array  contains  the  encoded
 *         constant  C  to jump from  X(n)  to  X(n+JUMPM) in the random
 *         sequence. ICM(0:1)  contains  respectively  the  16-lower and
 *         15-higher  bits  of this constant  C. When  JUMPM  is less or
 *         equal than zero, this array is not referenced.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   int                        j[2], k;
/* ..
 * .. Executable Statements ..
 */
   if( JUMPM > 0 )
   {
      IAM[0] = MULT[0]; IAM[1] = MULT[1];   /* IAM   = MULT;          */
      ICM[0] = IADD[0]; ICM[1] = IADD[1];   /* ICM   = IADD;          */
      for( k = 1; k <= JUMPM-1; k++ )
      {
         HPL_lmul( IAM, MULT, j );          /* j     = IAM   * MULT;  */
         IAM[0] = j[0]; IAM[1] = j[1];      /* IAM   = j;             */
         HPL_lmul( ICM, MULT, j );          /* j     = ICM   * MULT;  */
         HPL_ladd( IADD, j, ICM );          /* ICM   = IADD  + j;     */
      }
      HPL_lmul( IRANN, IAM, j );            /* j     = IRANN * IAM;   */
      HPL_ladd( j, ICM, IRANM );            /* IRANM = j     + ICM;   */
   }
   else
   {                                        /* IRANM = IRANN          */
      IRANM[0] = IRANN[0]; IRANM[1] = IRANN[1];
   }
/*
 * End of HPL_xjumpm
 */
}
