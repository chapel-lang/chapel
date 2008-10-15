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
void HPL_pdrpanllN
(
   HPL_T_panel *                    PANEL,
   const int                        M,
   const int                        N,
   const int                        ICOFF,
   double *                         WORK
)
#else
void HPL_pdrpanllN
( PANEL, M, N, ICOFF, WORK )
   HPL_T_panel *                    PANEL;
   const int                        M;
   const int                        N;
   const int                        ICOFF;
   double *                         WORK;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_pdrpanllN recursively  factorizes  a panel  of columns using  the
 * recursive Left-looking variant of the one-dimensional algorithm.  The
 * lower triangular  N0-by-N0  upper block  of  the  panel  is stored in
 * no-transpose form (i.e. just like the input matrix itself).
 *  
 * Bi-directional  exchange  is  used  to  perform  the  swap::broadcast
 * operations  at once  for one column in the panel.  This  results in a
 * lower number of slightly larger  messages than usual.  On P processes
 * and assuming bi-directional links,  the running time of this function
 * can be approximated by (when N is equal to N0):                      
 *  
 *    N0 * log_2( P ) * ( lat + ( 2*N0 + 4 ) / bdwth ) +
 *    N0^2 * ( M - N0/3 ) * gam2-3
 *  
 * where M is the local number of rows of  the panel, lat and bdwth  are
 * the latency and bandwidth of the network for  double  precision  real
 * words,  and  gam2-3  is an estimate of the  Level 2 and Level 3  BLAS
 * rate of execution. The  recursive  algorithm  allows indeed to almost
 * achieve  Level 3 BLAS  performance  in the panel factorization.  On a
 * large  number of modern machines,  this  operation is however latency
 * bound,  meaning  that its cost can  be estimated  by only the latency
 * portion N0 * log_2(P) * lat.  Mono-directional links will double this
 * communication cost.
 *
 * Arguments
 * =========
 *
 * PANEL   (local input/output)          HPL_T_panel *
 *         On entry,  PANEL  points to the data structure containing the
 *         panel information.
 *
 * M       (local input)                 const int
 *         On entry,  M specifies the local number of rows of sub(A).
 *
 * N       (local input)                 const int
 *         On entry,  N specifies the local number of columns of sub(A).
 *
 * ICOFF   (global input)                const int
 *         On entry, ICOFF specifies the row and column offset of sub(A)
 *         in A.
 *
 * WORK    (local workspace)             double *
 *         On entry, WORK  is a workarray of size at least 2*(4+2*N0).
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   double                     * A, * Aptr, * L1, * L1ptr;
#ifdef HPL_CALL_VSIPL
   vsip_mview_d               * Av0, * Lv0, * Av1, * Av2, * Lv1;
#endif
   int                        curr, ii, ioff, jb, jj, lda, m, n, n0, nb,
                              nbdiv, nbmin;
/* ..
 * .. Executable Statements ..
 */
   if( N <= ( nbmin = PANEL->algo->nbmin ) )
   { PANEL->algo->pffun( PANEL, M, N, ICOFF, WORK ); return; }
/*
 * Find  new recursive blocking factor.  To avoid an infinite loop,  one
 * must guarantee: 1 <= jb < N, knowing that  N  is greater than  NBMIN.
 * First, we compute nblocks:  the number of blocks of size  NBMIN in N,
 * including the last one that may be smaller.  nblocks  is thus  larger
 * than or equal to one, since N >= NBMIN.
 * The ratio ( nblocks + NDIV - 1 ) / NDIV  is thus larger than or equal
 * to one as  well.  For  NDIV >= 2,  we  are guaranteed  that the quan-
 * tity ( ( nblocks + NDIV  - 1 ) / NDIV ) * NBMIN  is less  than N  and
 * greater than or equal to NBMIN.
 */
   nbdiv = PANEL->algo->nbdiv; ii = jj = 0; m = M; n = N;
   nb = jb = ( (((N+nbmin-1) / nbmin) + nbdiv  - 1) / nbdiv ) * nbmin;
 
   A     = PANEL->A;   lda = PANEL->lda;
   L1    = PANEL->L1;  n0  = PANEL->jb;
   L1ptr = Mptr( L1, ICOFF, ICOFF, n0 );
   curr  = (int)( PANEL->grid->myrow == PANEL->prow );

   if( curr != 0 ) Aptr = Mptr( A, ICOFF, ICOFF, lda );
   else            Aptr = Mptr( A,     0, ICOFF, lda );
/*
 * The triangular solve is replicated in every  process row.  The  panel
 * factorization is  such that  the first rows of  A  are accumulated in
 * every process row during the (panel) swapping phase.  We  ensure this
 * way a minimum amount  of communication during the entire panel facto-
 * rization.
 */
   do
   {
      n -= jb; ioff = ICOFF + jj;
/*
 * Replicated solve - Local update - Factor current panel
 */
      HPL_dtrsm( HplColumnMajor, HplLeft, HplLower, HplNoTrans, HplUnit,
                 jj, jb, HPL_rone, L1ptr, n0, Mptr( L1ptr, 0, jj, n0 ),
                 n0 );
#ifdef HPL_CALL_VSIPL
/*
 * Admit the blocks
 */
      (void) vsip_blockadmit_d( PANEL->Ablock,  VSIP_TRUE );
      (void) vsip_blockadmit_d( PANEL->L1block, VSIP_TRUE );
/*
 * Create the matrix views
 */
      Av0 = vsip_mbind_d( PANEL->Ablock,  0, 1, lda, lda, PANEL->pmat->nq );
      Lv0 = vsip_mbind_d( PANEL->L1block, 0, 1,  n0,  n0, n0              );
/*
 * Create the matrix subviews
 */
      if( curr != 0 )
      {
         Av1 = vsip_msubview_d( Av0, PANEL->ii+ICOFF+ii, PANEL->jj+ICOFF,
                                m, jj );
         Av2 = vsip_msubview_d( Av0, PANEL->ii+ICOFF+ii, PANEL->jj+ioff,
                                m, jj );
      }
      else
      {
         Av1 = vsip_msubview_d( Av0, PANEL->ii+ii, PANEL->jj+ICOFF, m,  jj );
         Av2 = vsip_msubview_d( Av0, PANEL->ii+ii, PANEL->jj+ioff,  m,  jj );
      }
      Lv1 = vsip_msubview_d( Lv0, ICOFF, ioff, jj, jb );

      vsip_gemp_d( -HPL_rone, Av1, VSIP_MAT_NTRANS, Lv1, VSIP_MAT_NTRANS,
                   HPL_rone, Av2 );
/*
 * Destroy the matrix subviews
 */
      (void) vsip_mdestroy_d( Lv1 ); 
      (void) vsip_mdestroy_d( Av2 );
      (void) vsip_mdestroy_d( Av1 );
/*
 * Release the blocks
 */
      (void) vsip_blockrelease_d( vsip_mgetblock_d( Lv0 ), VSIP_TRUE );
      (void) vsip_blockrelease_d( vsip_mgetblock_d( Av0 ), VSIP_TRUE );
/*
 * Destroy the matrix views
 */
      (void) vsip_mdestroy_d( Lv0 );
      (void) vsip_mdestroy_d( Av0 );
#else
      HPL_dgemm( HplColumnMajor, HplNoTrans, HplNoTrans, m, jb,
                 jj, -HPL_rone, Mptr( Aptr, ii, 0, lda ), lda,
                 Mptr( L1ptr, 0, jj, n0 ), n0, HPL_rone,
                 Mptr( Aptr, ii, jj, lda ), lda );
#endif
      HPL_pdrpanllN( PANEL, m, jb, ioff, WORK );
/*
 * Copy back upper part of A in current process row - Go the next block
 */
      if( curr != 0 )
      {
         HPL_dlacpy( ioff, jb, Mptr( L1, 0, ioff, n0 ), n0,
                     Mptr( A, 0, ioff, lda ), lda );
         ii += jb; m -= jb;
      }
      jj += jb; jb = Mmin( n, nb );

   } while( n > 0 );
/*
 * End of HPL_pdrpanllN
 */
}
