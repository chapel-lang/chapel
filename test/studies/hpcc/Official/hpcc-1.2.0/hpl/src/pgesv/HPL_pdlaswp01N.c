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
void HPL_pdlaswp01N
(
   HPL_T_panel *                    PBCST,
   int *                            IFLAG,
   HPL_T_panel *                    PANEL,
   const int                        NN
)
#else
void HPL_pdlaswp01N
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
 * HPL_pdlaswp01N applies the  NB  row interchanges to  NN columns of the
 * trailing submatrix and broadcast a column panel.
 *  
 * A "Spread then roll" algorithm performs  the swap :: broadcast  of the
 * row panel U at once,  resulting in a minimal communication volume  and
 * a "very good"  use of the connectivity if available.  With  P  process
 * rows  and  assuming  bi-directional links,  the  running time  of this
 * function can be approximated by:
 *  
 *    (log_2(P)+(P-1)) * lat +   K * NB * LocQ(N) / bdwth
 *  
 * where  NB  is the number of rows of the row panel U,  N is the global
 * number of columns being updated,  lat and bdwth  are the latency  and
 * bandwidth  of  the  network  for  double  precision real words.  K is
 * a constant in (2,3] that depends on the achieved bandwidth  during  a
 * simultaneous  message exchange  between two processes.  An  empirical
 * optimistic value of K is typically 2.4.
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
 *         panel information.
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
   double                    * A, * U;
   int                       * ipID, * iplen, * ipmap, * ipmapm1,
                             * iwork, * lindxA = NULL, * lindxAU,
                             * permU;
   static int                equil=-1;
   int                       icurrow, * iflag, * ipA, * ipl, jb, k,
                             lda, myrow, n, nprow;
#define LDU                  jb
/* ..
 * .. Executable Statements ..
 */
   n = PANEL->n; n = Mmin( NN, n ); jb = PANEL->jb;
/*
 * Quick return if there is nothing to do
 */
   if( ( n <= 0 ) || ( jb <= 0 ) ) return;
#ifdef HPL_DETAILED_TIMING
   HPL_ptimer( HPL_TIMING_LASWP );
#endif
/*
 * Decide whether equilibration should be performed or not
 */
   if( equil == -1 ) equil = PANEL->algo->equil;
/*
 * Retrieve parameters from the PANEL data structure
 */
   nprow = PANEL->grid->nprow; myrow = PANEL->grid->myrow;
   A     = PANEL->A;   U       = PANEL->U;     iflag  = PANEL->IWORK;
   lda   = PANEL->lda; icurrow = PANEL->prow;
/*
 * Compute ipID (if not already done for this panel). lindxA and lindxAU
 * are of length at most 2*jb - iplen is of size nprow+1, ipmap, ipmapm1
 * are of size nprow,  permU is of length jb, and  this function needs a 
 * workspace of size max( 2 * jb (plindx1), nprow+1(equil)): 
 * 1(iflag) + 1(ipl) + 1(ipA) + 9*jb + 3*nprow + 1 + MAX(2*jb,nprow+1)
 * i.e. 4 + 9*jb + 3*nprow + max(2*jb, nprow+1);
 */
   k = (int)((unsigned int)(jb) << 1);  ipl = iflag + 1; ipID = ipl + 1;
   ipA     = ipID + ((unsigned int)(k) << 1); lindxA = ipA + 1;
   lindxAU = lindxA + k; iplen = lindxAU + k; ipmap = iplen + nprow + 1;
   ipmapm1 = ipmap + nprow; permU = ipmapm1 + nprow; iwork = permU + jb;

   if( *iflag == -1 )    /* no index arrays have been computed so far */
   {
      HPL_pipid(   PANEL,  ipl, ipID );
      HPL_plindx1( PANEL, *ipl, ipID, ipA, lindxA, lindxAU, iplen,
                   ipmap, ipmapm1, permU, iwork );
      *iflag = 1;
   }
   else if( *iflag == 0 ) /* HPL_pdlaswp00N called before: reuse ipID */
   {
      HPL_plindx1( PANEL, *ipl, ipID, ipA, lindxA, lindxAU, iplen,
                   ipmap, ipmapm1, permU, iwork );
      *iflag = 1;
   }
   else if( ( *iflag == 1 ) && ( equil != 0 ) )
   {   /* HPL_pdlaswp01N was call before only re-compute IPLEN, IPMAP */
      HPL_plindx10( PANEL, *ipl, ipID, iplen, ipmap, ipmapm1 );
      *iflag = 1;
   }
/*
 * Copy into U the rows to be spread (local to icurrow)
 */
   if( myrow == icurrow )
   { HPL_dlaswp01N( *ipA, n, A, lda, U, LDU, lindxA, lindxAU ); }
/*
 * Spread U - optionally probe for column panel
 */
   HPL_spreadN( PBCST, IFLAG, PANEL, HplRight, n, U, LDU, 0, iplen,
                ipmap, ipmapm1 );
/*
 * Local exchange (everywhere but in process row icurrow)
 */
   if( myrow != icurrow )
   {
      k = ipmapm1[myrow];
      HPL_dlaswp06N( iplen[k+1]-iplen[k], n, A, lda, Mptr( U, iplen[k],
                     0, LDU ), LDU, lindxA );
   }
/*
 * Equilibration
 */
   if( equil != 0 )
      HPL_equil( PBCST, IFLAG, PANEL, HplNoTrans, n, U, LDU, iplen,
                 ipmap, ipmapm1, iwork );
/*
 * Rolling phase
 */
   HPL_rollN( PBCST, IFLAG, PANEL, n, U, LDU, iplen, ipmap, ipmapm1 );
/*
 * Permute U in every process row
 */
   HPL_dlaswp00N( jb, n, U, LDU, permU );

#ifdef HPL_DETAILED_TIMING
   HPL_ptimer( HPL_TIMING_LASWP );
#endif
/*
 * End of HPL_pdlaswp01N
 */
}
