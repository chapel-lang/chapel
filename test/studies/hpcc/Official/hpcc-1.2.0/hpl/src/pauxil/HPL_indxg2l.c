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
int HPL_indxg2l
(
   const int                        IG,
   const int                        INB,
   const int                        NB,
   const int                        SRCPROC,
   const int                        NPROCS
)
#else
int HPL_indxg2l
( IG, INB, NB, SRCPROC, NPROCS )
   const int                        IG;
   const int                        INB;
   const int                        NB;
   const int                        SRCPROC;
   const int                        NPROCS;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_indxg2l computes  the local index of a matrix entry pointed to by
 * the  global index IG.  This  local  returned index is the same in all
 * processes.
 *
 * Arguments
 * =========
 *
 * IG      (input)                       const int
 *         On entry, IG specifies the global index of the matrix  entry.
 *         IG must be at least zero.
 *
 * INB     (input)                       const int
 *         On entry,  INB  specifies  the size of the first block of the
 *         global matrix. INB must be at least one.
 *
 * NB      (input)                       const int
 *         On entry,  NB specifies the blocking factor used to partition
 *         and distribute the matrix. NB must be larger than one.
 *
 * SRCPROC (input)                       const int
 *         On entry, if SRCPROC = -1, the data  is not  distributed  but
 *         replicated,  in  which  case  this  routine returns IG in all
 *         processes. Otherwise, the value of SRCPROC is ignored.
 *
 * NPROCS  (input)                       const int
 *         On entry,  NPROCS  specifies the total number of process rows
 *         or columns over which the matrix is distributed.  NPROCS must
 *         be at least one.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   int                        i, j;
/* ..
 * .. Executable Statements ..
 */
   if( ( IG < INB ) || ( SRCPROC == -1 ) || ( NPROCS == 1 ) )
/*
 * IG  belongs  to the first block,  or the data is not distributed,  or
 * there is just one process in this dimension of the grid.
 */
      return( IG );
/*
 * IG  =  INB - NB + ( l * NPROCS + MYROC ) * NB + X  with  0 <= X < NB,
 * thus IG is to be found in the block (IG-INB+NB) / NB = l*NPROCS+MYROC
 * with  0 <= MYROC < NPROCS.  The local index to be returned depends on 
 * whether  IG  resides in the process owning the first partial block of
 * size INB (MYROC=0). To determine this cheaply, let i = (IG-INB) / NB,
 * so that if NPROCS divides i+1, i.e. MYROC=0,  we have i+1 = l*NPROCS.
 * If we set  j = i / NPROCS, it follows that j = l-1. Therefore, i+1 is
 * equal to (j+1) * NPROCS.  Conversely, if NPROCS does not divide  i+1, 
 * then i+1 = l*NPROCS + MYROC with 1 <= MYROC < NPROCS. It follows that
 * j=l and thus (j+1)*NPROCS > i+1.
 */
   j = ( i = ( IG - INB ) / NB ) / NPROCS;
/*
 * When IG resides in the process owning the first partial block of size
 * INB (MYROC = 0), then the result IL can be written as:
 * IL = INB - NB + l * NB + X  = IG + ( l - (l * NPROCS + MYROC) ) * NB.
 * Using the above notation,  we have i+1 = l*NPROCS + MYROC = l*NPROCS,
 * i.e l = ( i+1 ) / NPROCS = j+1,  since  NPROCS divides i+1, therefore
 * IL = IG + ( j + 1 - ( i + 1 ) ) * NB.
 *
 * Otherwise when MYROC >= 1, the result IL can be written as:
 * IL = l * NB + X = IG - INB + ( ( l+1 ) - ( l * NPROCS + MYROC ) )*NB.
 * We still have i+1 = l*NPROCS+MYROC. Since NPROCS does not divide i+1,
 * we have j = (l*NPROCS+MYROC-1) / NPROCS = l, i.e
 * IL = IG - INB + ( j + 1 - ( i + 1 ) ) * NB.
 */
   return( NB * (j - i) + 
           ( ( i + 1 - ( j + 1 )*NPROCS ) ? IG - INB : IG ) );
/*
 * End of HPL_indxg2l
 */
}
