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

#ifdef HPL_NO_MPI_DATATYPE  /* The user insists to not use MPI types */
#ifndef HPL_COPY_L       /* and also want to avoid the copy of L ... */
#define HPL_COPY_L   /* well, sorry, can not do that: force the copy */
#endif
#endif

#ifdef HPL_STDC_HEADERS
int HPL_binit_2rinM
(
   HPL_T_panel *              PANEL
)
#else
int HPL_binit_2rinM( PANEL )
   HPL_T_panel *              PANEL;
#endif
{
#ifdef HPL_USE_MPI_DATATYPE
/*
 * .. Local Variables ..
 */
   int                        ierr;
#endif
/* ..
 * .. Executable Statements ..
 */
   if( PANEL == NULL )           { return( HPL_SUCCESS ); }
   if( PANEL->grid->npcol <= 1 ) { return( HPL_SUCCESS ); }
#ifdef HPL_USE_MPI_DATATYPE
#ifdef HPL_COPY_L
/*
 * Copy the panel into a contiguous buffer
 */
   HPL_copyL( PANEL );
#endif
/*
 * Create the MPI user-defined data type
 */
   ierr = HPL_packL( PANEL, 0, PANEL->len, 0 );
 
   return( ( ierr == MPI_SUCCESS ? HPL_SUCCESS : HPL_FAILURE ) );
#else
/*
 * Force the copy of the panel into a contiguous buffer
 */
   HPL_copyL( PANEL );
 
   return( HPL_SUCCESS );
#endif
}

#ifdef HPL_USE_MPI_DATATYPE
 
#define   _M_BUFF     PANEL->buffers[0]
#define   _M_COUNT    PANEL->counts[0]
#define   _M_TYPE     PANEL->dtypes[0]
 
#else
 
#define   _M_BUFF     (void *)(PANEL->L2)
#define   _M_COUNT    PANEL->len
#define   _M_TYPE     MPI_DOUBLE
 
#endif

#ifdef HPL_STDC_HEADERS
int HPL_bcast_2rinM
(
   HPL_T_panel                * PANEL,
   int                        * IFLAG
)
#else
int HPL_bcast_2rinM( PANEL, IFLAG )
   HPL_T_panel                * PANEL;
   int                        * IFLAG;
#endif
{
/*
 * .. Local Variables ..
 */
   MPI_Comm                   comm;
   int                        ierr, go, next, msgid, partner, prev,
                              rank, roo2, root, size;
/* ..
 * .. Executable Statements ..
 */
   if( PANEL == NULL ) { *IFLAG = HPL_SUCCESS; return( HPL_SUCCESS ); }
   if( ( size = PANEL->grid->npcol ) <= 1 )
   {                     *IFLAG = HPL_SUCCESS; return( HPL_SUCCESS ); }
/*
 * Cast phase: root process send to its two right neighbors and mid-pro-
 * cess. If I am not the root process, probe for message. If the message
 * is there, then receive it. If I am not the last process of both rings
 * then forward it to the next.  Otherwise,  inform  the caller that the
 * panel has still not been received.
 */
   rank = PANEL->grid->mycol;           comm  = PANEL->grid->row_comm;
   root = PANEL->pcol;                  msgid = PANEL->msgid;
   next = MModAdd1( rank, size );       roo2  = ( ( size + 1 ) >> 1 );
   roo2 = MModAdd(  root, roo2, size );
 
   if( rank == root )
   {
      ierr = MPI_Send( _M_BUFF, _M_COUNT, _M_TYPE, next, msgid, comm );

      if( ( ierr == MPI_SUCCESS ) && ( size > 2 ) )
      {
         if( MModAdd1( next, size ) != roo2 )
         {
            ierr = MPI_Send( _M_BUFF, _M_COUNT, _M_TYPE,
                             MModAdd1( next, size ), msgid, comm );
         }

         if( ierr == MPI_SUCCESS )
         {
            ierr = MPI_Send( _M_BUFF, _M_COUNT, _M_TYPE, roo2, msgid,
                             comm );
         }
      }
   }
   else
   {
      prev = MModSub1( rank, size );
      if( ( prev == root ) || ( rank == roo2 ) ||
          ( MModSub1( prev,  size )  == root ) ) partner = root;
      else                                       partner = prev;
 
      ierr = MPI_Iprobe( partner, msgid, comm, &go, &PANEL->status[0] );

      if( ierr == MPI_SUCCESS )
      {
         if( go != 0 )
         {
            ierr = MPI_Recv( _M_BUFF, _M_COUNT, _M_TYPE, partner, msgid,
                             comm, &PANEL->status[0] );
            if( ( ierr == MPI_SUCCESS ) && ( prev != root ) &&
                ( next != roo2        ) && ( next != root ) )
            {
               ierr = MPI_Send( _M_BUFF, _M_COUNT, _M_TYPE, next, msgid,
                                comm );
            }
         }
         else { *IFLAG = HPL_KEEP_TESTING; return( *IFLAG ); }
      }
   }
/*
 * If the message was received and being forwarded,  return HPL_SUCCESS.
 * If an error occured in an MPI call, return HPL_FAILURE.
 */
   *IFLAG = ( ierr == MPI_SUCCESS ? HPL_SUCCESS : HPL_FAILURE );

   return( *IFLAG );
} 

#ifdef HPL_STDC_HEADERS
int HPL_bwait_2rinM
(
   HPL_T_panel *              PANEL
)
#else
int HPL_bwait_2rinM( PANEL )
   HPL_T_panel *              PANEL;
#endif
{
#ifdef HPL_USE_MPI_DATATYPE
/*
 * .. Local Variables ..
 */
   int                        ierr;
#endif
/* ..
 * .. Executable Statements ..
 */
   if( PANEL == NULL )           { return( HPL_SUCCESS ); }
   if( PANEL->grid->npcol <= 1 ) { return( HPL_SUCCESS ); }
/*
 * Release the arrays of request / status / data-types and buffers
 */
#ifdef HPL_USE_MPI_DATATYPE
   ierr = MPI_Type_free( &PANEL->dtypes[0] );
 
   return( ( ierr == MPI_SUCCESS ? HPL_SUCCESS : HPL_FAILURE ) );
#else
   return( HPL_SUCCESS );
#endif
}
