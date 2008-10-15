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
void HPL_jumpit
(
   int *                            MULT,
   int *                            IADD,
   int *                            IRANN,
   int *                            IRANM
)
#else
void HPL_jumpit
( MULT, IADD, IRANN, IRANM )
   int *                            MULT;
   int *                            IADD;
   int *                            IRANN;
   int *                            IRANM;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_jumpit jumps in the random sequence from the number  X(n) encoded
 * in IRANN to the number  X(m)  encoded in  IRANM using the constants A
 * and C encoded in MULT and IADD: X(m) = A * X(n) + C.  The constants A
 * and C obviously depend on m and n,  see  the function  HPL_xjumpm  in
 * order to initialize them.
 *
 * Arguments
 * =========
 *
 * MULT    (local input)                 int *
 *         On entry, MULT is an array of dimension 2, that contains the
 *         16-lower and 15-higher bits of the constant A.
 *
 * IADD    (local input)                 int *
 *         On entry, IADD is an array of dimension 2, that contains the
 *         16-lower and 15-higher bits of the constant C.
 *
 * IRANN   (local input)                 int *
 *         On entry,  IRANN  is an array of dimension 2,  that contains 
 *         the 16-lower and 15-higher bits of the encoding of X(n).
 *
 * IRANM   (local output)                int *
 *         On entry,  IRANM  is an array of dimension 2.  On exit, this
 *         array contains respectively the 16-lower and  15-higher bits
 *         of the encoding of X(m).
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   int                          j[2];
/* ..
 * .. Executable Statements ..
 */
   HPL_lmul( IRANN, MULT, j );              /* j     = IRANN * MULT;  */
   HPL_ladd( j, IADD, IRANM );              /* IRANM = j     + IADD;  */
   HPL_setran( 0, IRANM );                  /* irand = IRANM          */
/*
 * End of HPL_jumpit
 */
}
