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
int HPL_grid_info
(
   const HPL_T_grid *               GRID,
   int *                            NPROW,
   int *                            NPCOL,
   int *                            MYROW,
   int *                            MYCOL
)
#else
int HPL_grid_info
( GRID, NPROW, NPCOL, MYROW, MYCOL )
   const HPL_T_grid *               GRID;
   int *                            NPROW;
   int *                            NPCOL;
   int *                            MYROW;
   int *                            MYCOL;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_grid_info returns  the grid shape and the coordinates in the grid
 * of the calling process.  Successful  completion  is  indicated by the
 * returned error code  MPI_SUCCESS. Other error codes depend on the MPI
 * implementation.
 *
 * Arguments
 * =========
 *
 * GRID    (local input)                 const HPL_T_grid *
 *         On entry,  GRID  points  to the data structure containing the
 *         process grid information.
 *
 * NPROW   (global output)               int *
 *         On exit,   NPROW  specifies the number of process rows in the
 *         grid. NPROW is at least one.
 *
 * NPCOL   (global output)               int *
 *         On exit,   NPCOL  specifies  the number of process columns in
 *         the grid. NPCOL is at least one.
 *
 * MYROW   (global output)               int *
 *         On exit,  MYROW  specifies my  row process  coordinate in the
 *         grid. MYROW is greater than or equal  to zero  and  less than
 *         NPROW.
 *
 * MYCOL   (global output)               int *
 *         On exit,  MYCOL specifies my column process coordinate in the
 *         grid. MYCOL is greater than or equal  to zero  and  less than
 *         NPCOL.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   *NPROW = GRID->nprow; *NPCOL = GRID->npcol;
   *MYROW = GRID->myrow; *MYCOL = GRID->mycol;
   return( MPI_SUCCESS );
/*
 * End of HPL_grid_info
 */
}
