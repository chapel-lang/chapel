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
int HPL_numroc
(
   const int                        N,
   const int                        INB,
   const int                        NB,
   const int                        PROC,
   const int                        SRCPROC,
   const int                        NPROCS
)
#else
int HPL_numroc
( N, INB, NB, PROC, SRCPROC, NPROCS )
   const int                        N;
   const int                        INB;
   const int                        NB;
   const int                        PROC;
   const int                        SRCPROC;
   const int                        NPROCS;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_numroc returns  the  local number of matrix rows/columns process
 * PROC  will  get  if  we give out  N rows/columns starting from global
 * index 0.
 *
 * Arguments
 * =========
 *
 * N       (input)                       const int
 *         On entry, N  specifies the number of rows/columns being dealt
 *         out. N must be at least zero.
 *
 * INB     (input)                       const int
 *         On entry,  INB  specifies  the size of the first block of the
 *         global matrix. INB must be at least one.
 *
 * NB      (input)                       const int
 *         On entry,  NB specifies the blocking factor used to partition
 *         and distribute the matrix A. NB must be larger than one.
 *
 * PROC    (input)                       const int
 *         On entry, PROC specifies  the coordinate of the process whose
 *         local portion is determined.  PROC must be at least zero  and
 *         strictly less than NPROCS.
 *
 * SRCPROC (input)                       const int
 *         On entry,  SRCPROC  specifies  the coordinate of the  process
 *         that possesses the first row or column of the matrix. SRCPROC
 *         must be at least zero and strictly less than NPROCS.
 *
 * NPROCS  (input)                       const int
 *         On entry,  NPROCS  specifies the total number of process rows
 *         or columns over which the matrix is distributed.  NPROCS must
 *         be at least one.
 *
 * ---------------------------------------------------------------------
 */ 
/* ..
 * .. Executable Statements ..
 */
   return( HPL_numrocI( N, 0, INB, NB, PROC, SRCPROC, NPROCS ) );
/*
 * End of HPL_numroc
 */
}
