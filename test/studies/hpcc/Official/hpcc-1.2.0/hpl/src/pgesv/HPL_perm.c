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
void HPL_perm
(
   const int                        N,
   int *                            LINDXA,
   int *                            LINDXAU,
   int *                            IWORK
)
#else
void HPL_perm
( N, LINDXA, LINDXAU, IWORK )
   const int                        N;
   int *                            LINDXA;
   int *                            LINDXAU;
   int *                            IWORK;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_perm combines  two  index  arrays  and generate the corresponding
 * permutation. First, this function computes the inverse of LINDXA, and
 * then combine it with LINDXAU.  Second, in order to be able to perform
 * the permutation in place,  LINDXAU  is overwritten by the sequence of
 * permutation  producing  the  same result.  What we ultimately want to
 * achieve is:  U[LINDXAU[i]] := U[LINDXA[i]] for i in [0..N). After the
 * call to this function,  this in place permutation can be performed by
 * for i in [0..N) swap U[i] with U[LINDXAU[i]].
 *
 * Arguments
 * =========
 *
 * N       (global input)                const int
 *         On entry,  N  specifies the length of the arrays  LINDXA  and
 *         LINDXAU. N should be at least zero.
 *
 * LINDXA  (global input/output)         int *
 *         On entry,  LINDXA  is an array of dimension N  containing the
 *         source indexes. On exit,  LINDXA  contains the combined index
 *         array.
 *
 * LINDXAU (global input/output)         int *
 *         On entry,  LINDXAU is an array of dimension N  containing the
 *         target indexes.  On exit,  LINDXAU  contains  the sequence of
 *         permutation,  that  should be applied  in increasing order to
 *         permute the underlying array U in place.
 *
 * IWORK   (workspace)                   int *
 *         On entry, IWORK is a workarray of dimension N.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   int                        i, j, k, fndd;
/* ..
 * .. Executable Statements ..
 */
/*
 * Inverse LINDXA - combine LINDXA and LINDXAU - Initialize IWORK
 */
   for( i = 0; i < N; i++ ) { IWORK[LINDXA[i]] = i; }
   for( i = 0; i < N; i++ ) { LINDXA[i] = LINDXAU[IWORK[i]]; IWORK[i] = i; }
 
   for( i = 0; i < N; i++ )
   {
      /* search LINDXA such that    LINDXA[j]  == i */
      j = 0; do { fndd = ( LINDXA[j] == i ); j++; } while( !fndd ); j--;
      /* search IWORK  such that    IWORK[k]   == j */
      k = 0; do { fndd = ( IWORK[k]  == j ); k++; } while( !fndd ); k--;
      /* swap IWORK[i] and IWORK[k]; LINDXAU[i] = k */
      j = IWORK[i]; IWORK[i] = IWORK[k]; IWORK[k] = j;
      LINDXAU[i] = k;
   }
/*
 * End of HPL_perm
 */
}
