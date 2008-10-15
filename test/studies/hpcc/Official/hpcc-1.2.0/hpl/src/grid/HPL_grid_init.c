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
int HPL_grid_init
(
   MPI_Comm                         COMM,
   const HPL_T_ORDER                ORDER,
   const int                        NPROW,
   const int                        NPCOL,
   HPL_T_grid *                     GRID
)
#else
int HPL_grid_init
( COMM, ORDER, NPROW, NPCOL, GRID )
   MPI_Comm                         COMM;
   const HPL_T_ORDER                ORDER;
   const int                        NPROW;
   const int                        NPCOL;
   HPL_T_grid *                     GRID;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_grid_init creates a NPROW x NPCOL  process  grid using column- or
 * row-major ordering from an initial collection of processes identified
 * by an  MPI  communicator.  Successful  completion is indicated by the
 * returned error code MPI_SUCCESS.  Other error codes depend on the MPI
 * implementation. The coordinates of processes that are not part of the
 * grid are set to values outside of [0..NPROW) x [0..NPCOL).
 *
 * Arguments
 * =========
 *
 * COMM    (global/local input)          MPI_Comm
 *         On entry,  COMM  is  the  MPI  communicator  identifying  the
 *         initial  collection  of  processes out of which  the  grid is
 *         formed.
 *
 * ORDER   (global input)                const HPL_T_ORDER
 *         On entry, ORDER specifies how the processes should be ordered
 *         in the grid as follows:
 *            ORDER = HPL_ROW_MAJOR    row-major    ordering;
 *            ORDER = HPL_COLUMN_MAJOR column-major ordering;
 *
 * NPROW   (global input)                const int
 *         On entry,  NPROW  specifies the number of process rows in the
 *         grid to be created. NPROW must be at least one.
 *
 * NPCOL   (global input)                const int
 *         On entry,  NPCOL  specifies  the number of process columns in
 *         the grid to be created. NPCOL must be at least one.
 *
 * GRID    (local input/output)          HPL_T_grid *
 *         On entry,  GRID  points  to the data structure containing the
 *         process grid information to be initialized.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   int                        hdim, hplerr=MPI_SUCCESS, ierr, ip2, k,
                              mask, mycol, myrow, nprocs, rank, size;
/* ..
 * .. Executable Statements ..
 */
   MPI_Comm_rank( COMM, &rank ); MPI_Comm_size( COMM, &size );
/*
 * Abort if illegal process grid
 */
   nprocs = NPROW * NPCOL;
   if( ( nprocs > size ) || ( NPROW < 1 ) || ( NPCOL < 1 ) )
   { HPL_pabort( __LINE__, "HPL_grid_init", "Illegal Grid" ); }
/*
 * Row- or column-major ordering of the processes
 */
   if( ORDER == HPL_ROW_MAJOR )
   {
      GRID->order = HPL_ROW_MAJOR;
      myrow = rank / NPCOL; mycol = rank - myrow * NPCOL;
   }
   else
   {
      GRID->order = HPL_COLUMN_MAJOR;
      mycol = rank / NPROW; myrow = rank - mycol * NPROW;
   }
   GRID->iam   = rank;  GRID->myrow = myrow; GRID->mycol  = mycol;
   GRID->nprow = NPROW; GRID->npcol = NPCOL; GRID->nprocs = nprocs;
/*
 * row_ip2   : largest power of two <= nprow;
 * row_hdim  : row_ip2 procs hypercube dim;
 * row_ip2m1 : largest power of two <= nprow-1;
 * row_mask  : row_ip2m1 procs hypercube mask;
 */
   hdim = 0; ip2 = 1; k = NPROW;
   while( k > 1 ) { k >>= 1; ip2 <<= 1; hdim++; }
   GRID->row_ip2 = ip2; GRID->row_hdim = hdim; 

   mask = ip2 = 1;    k = NPROW - 1;
   while( k > 1 ) { k >>= 1; ip2 <<= 1; mask <<= 1; mask++; }
   GRID->row_ip2m1 = ip2; GRID->row_mask = mask; 
/*
 * col_ip2   : largest power of two <= npcol;
 * col_hdim  : col_ip2 procs hypercube dim;
 * col_ip2m1 : largest power of two <= npcol-1;
 * col_mask  : col_ip2m1 procs hypercube mask;
 */
   hdim = 0; ip2 = 1; k = NPCOL;
   while( k > 1 ) { k >>= 1; ip2 <<= 1; hdim++; }
   GRID->col_ip2 = ip2; GRID->col_hdim = hdim; 

   mask = ip2 = 1;    k = NPCOL - 1;
   while( k > 1 ) { k >>= 1; ip2 <<= 1; mask <<= 1; mask++; }
   GRID->col_ip2m1 = ip2; GRID->col_mask = mask; 
/*
 * All communicator, leave if I am not part of this grid. Creation of the
 * row- and column communicators.
 */
   ierr = MPI_Comm_split( COMM, ( rank < nprocs ? 0 : MPI_UNDEFINED ),
                          rank, &(GRID->all_comm) );
   if( GRID->all_comm == MPI_COMM_NULL ) return( ierr );

   ierr = MPI_Comm_split( GRID->all_comm, myrow, mycol, &(GRID->row_comm) );
   if( ierr != MPI_SUCCESS ) hplerr = ierr;

   ierr = MPI_Comm_split( GRID->all_comm, mycol, myrow, &(GRID->col_comm) );
   if( ierr != MPI_SUCCESS ) hplerr = ierr;

   return( hplerr );
/*
 * End of HPL_grid_init
 */
}
