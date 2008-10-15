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
int HPL_grid_exit
(
   HPL_T_grid *                     GRID
)
#else
int HPL_grid_exit
( GRID )
   HPL_T_grid *                     GRID;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_grid_exit marks  the process  grid object for  deallocation.  The
 * returned  error  code  MPI_SUCCESS  indicates  successful completion.
 * Other error codes are (MPI) implementation dependent.
 *
 * Arguments
 * =========
 *
 * GRID    (local input/output)          HPL_T_grid *
 *         On entry,  GRID  points  to the data structure containing the
 *         process grid to be released.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   int                        hplerr = MPI_SUCCESS, mpierr;
/* ..
 * .. Executable Statements ..
 */
   if( GRID->all_comm != MPI_COMM_NULL )
   {
      mpierr = MPI_Comm_free( &(GRID->row_comm) );
      if( mpierr != MPI_SUCCESS ) hplerr = mpierr;
      mpierr = MPI_Comm_free( &(GRID->col_comm) );
      if( mpierr != MPI_SUCCESS ) hplerr = mpierr;
      mpierr = MPI_Comm_free( &(GRID->all_comm) );
      if( mpierr != MPI_SUCCESS ) hplerr = mpierr;
   }

   GRID->order   = HPL_COLUMN_MAJOR;

   GRID->iam     = GRID->myrow    = GRID->mycol     = -1;
   GRID->nprow   = GRID->npcol    = GRID->nprocs    = -1;

   GRID->row_ip2 = GRID->row_hdim = GRID->row_ip2m1 = GRID->row_mask = -1;
   GRID->col_ip2 = GRID->col_hdim = GRID->col_ip2m1 = GRID->col_mask = -1;

   return( hplerr );
/*
 * End of HPL_grid_exit
 */
}
