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
void HPL_pdlaswp00N
(
   HPL_T_panel *                    PBCST,
   int *                            IFLAG,
   HPL_T_panel *                    PANEL,
   const int                        NN
)
#else
void HPL_pdlaswp00N
( PBCST, IFLAG, PANEL, NN )
   HPL_T_panel *                    PBCST;
   int *                            IFLAG;
   HPL_T_panel *                    PANEL;
   const int                        NN;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_pdlaswp00N applies the  NB  row interchanges to  NN columns of the
 * trailing submatrix and broadcast a column panel.
 *  
 * Bi-directional  exchange  is used to perform the  swap :: broadcast of
 * the row  panel U at once, resulting in a lower number of messages than
 * usual as well as a lower communication volume. With P process rows and
 * assuming  bi-directional links,  the running time of this function can
 * be approximated by:
 *  
 *    log_2(P) * (lat + NB*LocQ(N) / bdwth)
 *  
 * where  NB  is the number of rows of the row panel U,  N is the global
 * number of columns being updated,  lat and bdwth  are the latency  and
 * bandwidth  of  the  network  for  double  precision real words.  Mono
 * directional links will double this communication cost.
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
 *         panel (to be broadcast and swapped) information.
 *
 * NN      (local input)                 const int
 *         On entry, NN specifies  the  local  number  of columns of the
 *         trailing  submatrix  to  be swapped and broadcast starting at
 *         the current position. NN must be at least zero.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   MPI_Comm                  comm;
   HPL_T_grid                * grid;
   double                    * A, * U, * W;
   void                       * vptr = NULL;
   int                       * ipID, * lindxA, * lindxAU, * llen,
                             * llen_sv;
   unsigned int              ip2, ip2_=1, ipdist, ipow=1, mask=1,
                             mydist, mydis_;
   int                       Cmsgid=MSGID_BEGIN_PFACT, Np2, align,
                             hdim, i, icurrow, *iflag, ipA, ipW, *ipl,
                             iprow, jb, k, lda, ldW, myrow, n, nprow,
                             partner, root, size_, usize;
#define LDU                  jb
/* ..
 * .. Executable Statements ..
 */
   n = Mmin( NN, PANEL->n ); jb = PANEL->jb;
/*
 * Quick return if there is nothing to do
 */
   if( ( n <= 0 ) || ( jb <= 0 ) ) return;

#ifdef HPL_DETAILED_TIMING
   HPL_ptimer( HPL_TIMING_LASWP );
#endif
/*
 * Retrieve parameters from the PANEL data structure
 */
   grid  = PANEL->grid;    nprow   = grid->nprow; myrow = grid->myrow;
   comm  = grid->col_comm; ip2     = (unsigned int)grid->row_ip2;
   hdim  = grid->row_hdim; align   = PANEL->algo->align;
   A     = PANEL->A;       U       = PANEL->U;    iflag = PANEL->IWORK;
   lda   = PANEL->lda;     icurrow = PANEL->prow; usize = jb * n;
   ldW   = n + 1;
/*
 * Allocate space for temporary W (ldW * jb)
 */
   vptr = (void*)malloc( (align + (jb * ldW)) * sizeof(double) );
   if( vptr == NULL )
   { HPL_pabort( __LINE__, "HPL_pdlaswp00N", "Memory allocation failed" ); }

   W = (double *)HPL_PTR( vptr, ((size_t)(align) * sizeof(double) ) );
/*
 * Construct ipID and its local counter parts lindxA, lindxAU -  llen is
 * the number of rows/columns that I have in workspace and that I should
 * send.  Compute  lindx_, ipA, llen if it has not already been done for
 * this panel;
 */
   k = (int)((unsigned int)(jb) << 1); ipl = iflag + 1; ipID = ipl + 1;
   lindxA  = ipID + ((unsigned int)(k) << 1); lindxAU = lindxA + k;
   llen    = lindxAU + k; llen_sv = llen + nprow;

   if( *iflag == -1 )    /* no index arrays have been computed so far */
   {
      HPL_pipid(   PANEL,  ipl, ipID );
      HPL_plindx0( PANEL, *ipl, ipID, lindxA, lindxAU, llen_sv );
      *iflag = 0;
   }
   else if( *iflag == 1 ) /* HPL_pdlaswp01N called before: reuse ipID */
   {
      HPL_plindx0( PANEL, *ipl, ipID, lindxA, lindxAU, llen_sv );
      *iflag = 0;
   }
/*
 * Copy the llen_sv into llen - Reset ipA to its correct value
 */
   ipA = llen_sv[myrow];
   for( i = 0; i < nprow; i++ ) { llen[i]  = llen_sv[i]; }
/*
 * For i in [0..2*jb),  lindxA[i] is the offset in A of a row that ulti-
 * mately goes to U( lindxAU[i], : ) or U( :, lindxAU[i] ).  In icurrow,
 * we directly pack into U, otherwise we pack into workspace. The  first
 * entry of each column packed in workspace is in fact the row or column
 * offset in U where it should go to.
 */
   if( myrow == icurrow ) 
   {
      HPL_dlaswp01N( ipA, n, A, lda, U, LDU, lindxA, lindxAU );
   }
   else
   {
      HPL_dlaswp02N( ipA, n, A, lda, W, W+1, ldW, lindxA, lindxAU );
   }
/*
 * Probe for column panel - forward it when available 
 */
   if( *IFLAG == HPL_KEEP_TESTING ) (void) HPL_bcast( PBCST, IFLAG );
/*
 * Algorithm for bi-directional data exchange:
 *
 * As long as I have not talked to a process that  already  had the data
 * from icurrow,  I will be sending the workspace,  otherwise  I will be
 * sending U. Note that the columns in workspace contain the local index
 * in U they should go to.
 *
 * If I am receiving from a process that  has the data from  icurrow,  I
 * will be receiving in  U, copy the data of  U  that stays into  A, and
 * then the columns I have in workspace into U; otherwise  I will be re-
 * ceiving in the remaining workspace.  If I am one  of  those processes 
 * that already has the data from icurrow, I will be immediately copying
 * the data I have in my workspace into U.
 *
 * When I receive U, some of U should be copied in my piece of A  before
 * I can copy the rows I have in my workspace into  U.  This information
 * is kept in the lists  lindx_:  the row lindxAU[i] should be copied in
 * the row  lindxA[i] of my piece of  A, just as in the reversed initial
 * packing operation. Those rows are thus the first ones in the work ar-
 * ray.  After  this  operation  has  been  performed,  I will not  need
 * those lindx arrays,  and  I  will  always be sending a buffer of size
 * jb x n, or n x jb, that is, U.
 *
 * At  every  step  of  the algorithm, it is necesary to update the list 
 * llen,  so that I can figure out how large the next messages I will be
 * sending/receiving are.  It is  obvious when I am sending U. It is not
 * otherwise.
 *
 * We  choose  icurrow  to be the source of the bi-directional exchange.
 * This allows the processes in the non-power 2 part to receive U at the
 * first exchange,  and  then  broadcast internally this U so that those 
 * processes can grab their piece of A.
 */
   if( myrow == icurrow ) { llen[myrow] = 0; ipA = 0; }
   ipW    = ipA;
   Np2    = ( ( size_ = nprow - ip2 ) != 0 );
   mydist = (unsigned int)MModSub( myrow, icurrow, nprow );
/*
 * bi-directional exchange:   If nprow is not a power of 2,  proc[i-ip2]
 * receives local data from proc[i] for all i in  [ip2..nprow);  icurrow
 * is the source, these last process indexes are relative to icurrow.
 */
   if( ( Np2 != 0 ) && ( ( partner = (int)(mydist ^ ip2) ) < nprow ) )
   {
      partner = MModAdd( icurrow, partner, nprow );

      if( mydist == 0 )  /* I am the current row: I send U and recv W */
      {
         (void) HPL_sdrv( U, usize, Cmsgid, W, llen[partner] * ldW,
                          Cmsgid, partner, comm );
         if( llen[partner] > 0 )
            HPL_dlaswp03N( llen[partner], n, U, LDU, W, W+1, ldW );
      }
      else if( mydist == ip2 )
      {                      /* I recv U for later Bcast, I send my W */
         (void) HPL_sdrv( W, llen[myrow]*ldW, Cmsgid, U, usize,
                          Cmsgid, partner, comm );
      }
      else               /* None of us is icurrow, we exchange our Ws */
      {
         if( ( mydist & ip2 ) != 0 ) 
         {
            (void) HPL_send( W, llen[myrow]*ldW, partner, Cmsgid, comm );
         }
         else
         {
            (void) HPL_recv( Mptr( W, 0, ipW, ldW ), llen[partner]*ldW,
                             partner, Cmsgid, comm );
            if( llen[partner] > 0 ) ipW += llen[partner];
         }
      }
   }
/*
 * Update llen
 */
   for( i = 1; i < size_; i++ )
   {
      iprow   = MModAdd( icurrow, i,          nprow );
      partner = MModAdd( iprow,   (int)(ip2), nprow );
      llen[ iprow ] += llen[ partner ];
   }
/*
 * Probe for column panel - forward it when available 
 */
   if( *IFLAG == HPL_KEEP_TESTING ) (void) HPL_bcast( PBCST, IFLAG );
/*
 * power of 2 part of the processes collection:  only processes [0..ip2)
 * are working;  some of them  (mydist >> (k+1) == 0) either send or re-
 * ceive U.  At every step k, k is in [0 .. hdim),  of the algorithm,  a
 * process pair that exchanges  U  is such that  (mydist >> (k+1) == 0).
 * Among  those  processes,  the  ones  that are sending U are such that 
 * mydist >> k == 0.
 */
   if( mydist < ip2 )
   {
      k = 0;

      while( k < hdim )
      {
         partner = (int)(mydist ^ ipow);
         partner = MModAdd( icurrow, partner, nprow );
/*
 * Exchange and combine the local results - If I receive U,  then I must
 * copy from U the rows that belong to my piece of A, and then update  U
 * by  copying in it the rows I have accumulated in W.  Otherwise, I re-
 * ceive W.  In this later case, and I have U, I shall update my copy of
 * U by copying in it the rows I have accumulated in  W.  If  I  did not
 * have U before, I simply need to update my pointer in W for later use.
 */
         if( ( mydist >> (unsigned int)( k + 1 ) ) == 0 )
         {
            if( ( mydist >> (unsigned int)(k) ) == 0 )
            {
               (void) HPL_sdrv( U, usize, Cmsgid, Mptr( W, 0, ipW,
                                ldW ), llen[partner]*ldW, Cmsgid,
                                partner, comm );
               HPL_dlaswp03N( llen[partner], n, U, LDU, Mptr( W, 0, ipW,
                              ldW ), Mptr( W, 1, ipW, ldW ), ldW );
               ipW += llen[partner];
            }
            else
            {
               (void) HPL_sdrv( W, llen[myrow]*ldW, Cmsgid, U, usize,
                                Cmsgid, partner, comm );
               HPL_dlaswp04N( ipA, llen[myrow], n, U, LDU, A, lda, W,
                              W+1, ldW, lindxA, lindxAU );
            }
         }
         else
         {
            (void) HPL_sdrv( W, llen[myrow]*ldW, Cmsgid, Mptr( W, 0,
                             ipW, ldW ), llen[partner]*ldW, Cmsgid,
                             partner, comm );
            ipW += llen[partner];
         }
/*
 * Update llen - Go to next process pairs
 */
         iprow = icurrow; ipdist = 0;
         do
         {
            if( (unsigned int)( partner = (int)(ipdist ^ ipow) ) > ipdist )
            {
               partner = MModAdd( icurrow, partner, nprow );
               llen[iprow]  += llen[partner];
               llen[partner] = llen[iprow];
            }
            iprow = MModAdd( iprow, 1, nprow ); ipdist++;

         } while( ipdist < ip2 );

         ipow <<= 1; k++;
/*
 * Probe for column panel - forward it when available 
 */
         if( *IFLAG == HPL_KEEP_TESTING ) (void) HPL_bcast( PBCST, IFLAG );
      }
   }
   else
   {
/*
 * non power of 2 part of the process collection:  proc[ip2] broadcast U
 * to procs[ip2..nprow) (relatively to icurrow).
 */
      if( size_ > 1 )
      {
         k = size_ - 1;
         while( k > 1 ) { k >>= 1; ip2_ <<= 1; mask <<= 1; mask++; }
         root   = MModAdd( icurrow, (int)(ip2), nprow );
         mydis_ = (unsigned int)MModSub( myrow,  root, nprow );

         do
         {
            mask ^= ip2_;
            if( ( mydis_ & mask ) == 0 )
            {
               partner = (int)(mydis_ ^ ip2_);
               if( ( mydis_ & ip2_ ) != 0 )
               {
                  (void) HPL_recv( U, usize, MModAdd( root, partner,
                                   nprow ), Cmsgid, comm );

               }
               else if( partner < size_ )
               {
                  (void) HPL_send( U, usize, MModAdd( root, partner,
                                   nprow ), Cmsgid, comm );
               }
            }
            ip2_ >>= 1;
/*
 * Probe for column panel - forward it when available 
 */
            if( *IFLAG == HPL_KEEP_TESTING ) (void) HPL_bcast( PBCST, IFLAG );

         } while( ip2_ > 0 );
      }
/*
 * Every process in [ip2..nprow) (relatively to icurrow) grabs its piece
 * of A.
 */
      HPL_dlaswp05N( ipA, n, A, lda, U, LDU, lindxA, lindxAU );
   }
/*
 * If  nprow  is not a power of 2,  proc[i-ip2]  sends  global result to
 * proc[i] for all i in [ip2..nprow);
 */
   if( ( Np2 != 0 ) && ( ( partner = (int)(mydist ^ ip2) ) < nprow ) )
   {
      partner = MModAdd( icurrow, partner, nprow );
      if( ( mydist & ip2 ) != 0 )
      { (void) HPL_recv( U, usize, partner, Cmsgid, comm ); }
      else
      { (void) HPL_send( U, usize, partner, Cmsgid, comm ); }
   }

   if( vptr ) free( vptr );
/*
 * Probe for column panel - forward it when available 
 */
   if( *IFLAG == HPL_KEEP_TESTING ) (void) HPL_bcast( PBCST, IFLAG );

#ifdef HPL_DETAILED_TIMING
   HPL_ptimer( HPL_TIMING_LASWP );
#endif
/*
 * End of HPL_pdlaswp00N
 */
}
