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
void HPL_spreadN
(
   HPL_T_panel *                    PBCST,
   int *                            IFLAG,
   HPL_T_panel *                    PANEL,
   const enum HPL_SIDE              SIDE,
   const int                        N,
   double *                         U,
   const int                        LDU,
   const int                        SRCDIST,
   const int *                      IPLEN,
   const int *                      IPMAP,
   const int *                      IPMAPM1
)
#else
void HPL_spreadN
( PBCST, IFLAG, PANEL, SIDE, N, U, LDU, SRCDIST, IPLEN, IPMAP, IPMAPM1 )
   HPL_T_panel *                    PBCST;
   int *                            IFLAG;
   HPL_T_panel *                    PANEL;
   const enum HPL_SIDE              SIDE;
   const int                        N;
   double *                         U;
   const int                        LDU;
   const int                        SRCDIST;
   const int *                      IPLEN;
   const int *                      IPMAP;
   const int *                      IPMAPM1;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_spreadN spreads the local array containing local pieces of U, so
 * that on exit to this function,  a piece of  U  is contained in every
 * process row. The array IPLEN contains the number of rows of U,  that
 * should be spread on any given process row. This function also probes
 * for the presence of the column panel PBCST. In case of success, this
 * panel will be forwarded.  If  PBCST  is NULL on input,  this probing
 * mechanism will be disabled.
 *
 * Arguments
 * =========
 *
 * PBCST   (local input/output)          HPL_T_panel *
 *         On entry,  PBCST  points to the data structure containing the
 *         panel (to be broadcast) information.
 *
 * IFLAG   (local input/output)          int *
 *         On entry, IFLAG  indicates  whether or not  the broadcast has
 *         already been completed.  If not,  probing will occur, and the
 *         outcome will be contained in IFLAG on exit.
 *
 * PANEL   (local input/output)          HPL_T_panel *
 *         On entry,  PANEL  points to the data structure containing the
 *         panel (to be spread) information.
 *
 * SIDE    (global input)                const enum HPL_SIDE
 *         On entry, SIDE specifies whether the local piece of U located
 *         in process IPMAP[SRCDIST] should be spread to the right or to
 *         the left. This feature is used by the equilibration process.
 *
 * N       (global input)                const int
 *         On entry,  N  specifies  the  local number of columns of U. N
 *         must be at least zero.
 *
 * U       (local input/output)          double *
 *         On entry,  U  is an array of dimension (LDU,*) containing the
 *         local pieces of U.
 *
 * LDU     (local input)                 const int
 *         On entry, LDU specifies the local leading dimension of U. LDU
 *         should be at least MAX(1,IPLEN[nprow]).
 *
 * SRCDIST (local input)                 const int
 *         On entry,  SRCDIST  specifies the source process that spreads
 *         its piece of U.
 *
 * IPLEN   (global input)                const int *
 *         On entry, IPLEN is an array of dimension NPROW+1.  This array
 *         is such that IPLEN[i+1] - IPLEN[i] is the number of rows of U
 *         in each process before process IPMAP[i], with the  convention
 *         that IPLEN[nprow] is the total number of rows. In other words
 *         IPLEN[i+1] - IPLEN[i]  is  the local number of rows of U that
 *         should be moved to process IPMAP[i].
 *
 * IPMAP   (global input)                const int *
 *         On entry, IPMAP is an array of dimension  NPROW.  This  array
 *         contains  the  logarithmic mapping of the processes. In other
 *         words, IPMAP[myrow]  is the absolute coordinate of the sorted
 *         process.
 *
 * IPMAPM1 (global input)                const int *
 *         On entry,  IPMAPM1 is an array of dimension NPROW. This array
 *         contains  the inverse of the logarithmic mapping contained in
 *         IPMAP: For i in [0.. NPROW) IPMAPM1[IPMAP[i]] = i.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   MPI_Datatype              type;
   MPI_Status                status;
   MPI_Comm                  comm;
   unsigned int              ip2=1, mask=1, mydist, mydist2;
   int                       Cmsgid=MSGID_BEGIN_PFACT, ibuf,
                             ierr=MPI_SUCCESS, il, k, lbuf, lgth, myrow,
                             npm1, nprow, partner;
/* ..
 * .. Executable Statements ..
 */
   myrow = PANEL->grid->myrow;    nprow = PANEL->grid->nprow;
   comm  = PANEL->grid->col_comm;
/*
 * Spread U to the left
 */
   if( SIDE == HplLeft )
   {
      nprow = ( npm1 = SRCDIST ) + 1;
      if( ( ( mydist = (unsigned int)(IPMAPM1[myrow]) ) >
              (unsigned int)(SRCDIST) ) || ( npm1 == 0 ) ) return;

      k = npm1; while( k > 1 ) { k >>= 1; ip2 <<= 1; mask <<= 1; mask++; }
      mydist2 = ( mydist = npm1 - mydist ); il = npm1 - ip2;
      lgth    = IPLEN[nprow];

      do
      {
         mask ^= ip2;

         if( ( mydist & mask ) == 0 )
         {
            lbuf = IPLEN[il+1] - ( ibuf = IPLEN[il-Mmin(il, (int)(ip2))] ); 

            if( lbuf > 0 )
            {
               partner = mydist ^ ip2;

               if( mydist & ip2 )
               {
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Type_vector( N, lbuf, LDU, MPI_DOUBLE,
                                               &type );
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Type_commit( &type );
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Recv( Mptr( U, ibuf, 0, LDU ), 1, type,
                                        IPMAP[npm1-partner], Cmsgid, comm,
                                        &status );
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Type_free(   &type );
               }
               else if( partner < nprow )
               {
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Type_vector( N, lbuf, LDU, MPI_DOUBLE,
                                               &type );
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Type_commit( &type );
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Send( Mptr( U, ibuf, 0, LDU ), 1, type,
                                        IPMAP[npm1-partner], Cmsgid, comm );
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Type_free(   &type );
               }
            }
         }
 
         if( mydist2 < ip2 ) {  ip2 >>= 1; il += ip2; }
         else { mydist2 -= ip2; ip2 >>= 1; il -= ip2; }
/*
 * Probe for column panel - forward it when available
 */
         if( *IFLAG == HPL_KEEP_TESTING ) (void) HPL_bcast( PBCST, IFLAG );
 
      } while( ip2 > 0 );
   }
   else
   {
      npm1 = ( nprow -= SRCDIST ) - 1;
      if( ( ( mydist = (unsigned int)(IPMAPM1[myrow]) ) <
              (unsigned int)(SRCDIST) ) || ( npm1 == 0 ) ) return;

      k = npm1; while( k > 1 ) { k >>= 1; ip2 <<= 1; mask <<= 1; mask++; }
      mydist2 = ( mydist -= SRCDIST ); il = ip2;
      lgth    = IPLEN[SRCDIST+nprow];
/*
 * Spread U to the right - offset the IPLEN, and IPMAP arrays
 */
      do
      {
         mask ^= ip2;

         if( ( mydist & mask ) == 0 )
         {
            k    = il + ip2; ibuf = IPLEN[SRCDIST+il];
            lbuf = ( k >= nprow ? lgth : IPLEN[SRCDIST+k] ) - ibuf;

            if( lbuf > 0 )
            {
               partner = mydist ^ ip2;

               if( mydist & ip2 )
               {
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Type_vector( N, lbuf, LDU, MPI_DOUBLE,
                                               &type );
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Type_commit( &type );
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Recv( Mptr( U, ibuf, 0, LDU ), 1, type,
                                        IPMAP[SRCDIST+partner], Cmsgid,
                                        comm, &status );
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Type_free(   &type );
               }
               else if( partner < nprow )
               {
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Type_vector( N, lbuf, LDU, MPI_DOUBLE,
                                               &type );
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Type_commit( &type );
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Send( Mptr( U, ibuf, 0, LDU ), 1, type,
                                        IPMAP[SRCDIST+partner], Cmsgid,
                                        comm );
                  if( ierr == MPI_SUCCESS )  
                     ierr =   MPI_Type_free(   &type );
               }
            }
         }
 
         if( mydist2 < ip2 ) {  ip2 >>= 1; il -= ip2; }
         else { mydist2 -= ip2; ip2 >>= 1; il += ip2; }
/*
 * Probe for column panel - forward it when available
 */
         if( *IFLAG == HPL_KEEP_TESTING ) (void) HPL_bcast( PBCST, IFLAG );
 
      } while( ip2 > 0 );
   }

   if( ierr != MPI_SUCCESS )
   { HPL_pabort( __LINE__, "HPL_spreadN", "MPI call failed" ); }
/*
 * End of HPL_spreadN
 */
}
