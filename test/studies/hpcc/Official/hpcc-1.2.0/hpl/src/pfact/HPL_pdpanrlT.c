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
void HPL_pdpanrlT
(
   HPL_T_panel *                    PANEL,
   const int                        M,
   const int                        N,
   const int                        ICOFF,
   double *                         WORK
)
#else
void HPL_pdpanrlT
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
 * HPL_pdpanrlT factorizes  a panel of columns  that is a sub-array of a
 * larger one-dimensional panel A using the Right-looking variant of the
 * usual one-dimensional algorithm.  The lower triangular N0-by-N0 upper
 * block of the panel is stored in transpose form.
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
 * Note that  one  iteration of the the main loop is unrolled. The local
 * computation of the absolute value max of the next column is performed
 * just after its update by the current column. This allows to bring the
 * current column only  once through  cache at each  step.  The  current
 * implementation  does not perform  any blocking  for  this sequence of
 * BLAS operations, however the design allows for plugging in an optimal
 * (machine-specific) specialized  BLAS-like kernel.  This idea has been
 * suggested to us by Fred Gustavson, IBM T.J. Watson Research Center.
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
   double                     * A, * Acur, * Anxt, * L1;
#ifdef HPL_CALL_VSIPL
   vsip_mview_d               * Av0, * Av1, * Xv1, * Yv0, * Yv1;
#endif
   int                        Mm1, Nm1, curr, ii, iip1, jj, lda, m=M,
                              n0;
/* ..
 * .. Executable Statements ..
 */
#ifdef HPL_DETAILED_TIMING
   HPL_ptimer( HPL_TIMING_PFACT );
#endif
   A    = PANEL->A;   lda = PANEL->lda;
   L1   = PANEL->L1;  n0  = PANEL->jb;
   curr = (int)( PANEL->grid->myrow == PANEL->prow );

   Nm1  = N - 1; jj = ICOFF;
   if( curr != 0 ) { ii = ICOFF; iip1 = ii+1; Mm1 = m-1; }
   else            { ii = 0;     iip1 = ii;   Mm1 = m;   }
#ifdef HPL_CALL_VSIPL
/*
 * Admit the blocks
 */
   (void) vsip_blockadmit_d(  PANEL->Ablock,  VSIP_TRUE );
   (void) vsip_blockadmit_d(  PANEL->L1block, VSIP_TRUE );
/*
 * Create the matrix views
 */
   Av0 = vsip_mbind_d( PANEL->Ablock,  0, 1, lda,       lda, PANEL->pmat->nq );
   Yv0 = vsip_mbind_d( PANEL->L1block, 0, 1, PANEL->jb, PANEL->jb, PANEL->jb );
#endif
/*
 * Find local absolute value max in first column - initialize WORK[0:3]
 */
   HPL_dlocmax( PANEL, m, ii, jj, WORK );

   while( Nm1 >= 1 )
   {
      Acur = Mptr( A, iip1, jj, lda ); Anxt = Mptr( Acur, 0, 1, lda );
/*
 * Swap and broadcast the current row
 */
      HPL_pdmxswp(  PANEL, m, ii, jj, WORK );
      HPL_dlocswpT( PANEL,    ii, jj, WORK );
/*
 * Scale current column by its absolute value max entry  -  Update trai-
 * ling sub-matrix and find local absolute value max in next column (On-
 * ly one pass through cache for each current column).  This sequence of
 * operations could benefit from a specialized blocked implementation.
 */
      if( WORK[0] != HPL_rzero )
         HPL_dscal( Mm1, HPL_rone / WORK[0], Acur, 1 );
      HPL_daxpy( Mm1, -(*(Mptr( L1, jj+1, jj, n0 ))), Acur, 1, Anxt, 1 );
      HPL_dlocmax( PANEL, Mm1, iip1, jj+1, WORK );

      if( Nm1 > 1 )
      {
#ifdef HPL_CALL_VSIPL
/*
 * Create the matrix subviews
 */
         Av1 = vsip_msubview_d( Av0, PANEL->ii+iip1, PANEL->jj+jj+2,
                                Mm1, Nm1-1 );
         Xv1 = vsip_msubview_d( Av0, PANEL->ii+iip1, PANEL->jj+jj,
                                Mm1, 1   );
         Yv1 = vsip_msubview_d( Yv0, jj+2, jj, Nm1-1, 1 ); 

         vsip_gemp_d( -HPL_rone, Xv1, VSIP_MAT_NTRANS, Yv1, VSIP_MAT_TRANS,
                      HPL_rone, Av1 );
/*
 * Destroy the matrix subviews
 */
         (void) vsip_mdestroy_d( Yv1 );
         (void) vsip_mdestroy_d( Xv1 );
         (void) vsip_mdestroy_d( Av1 );
#else
         HPL_dger( HplColumnMajor, Mm1, Nm1-1, -HPL_rone, Acur, 1,
                   Mptr( L1, jj+2, jj, n0 ), 1, Mptr( Anxt, 0, 1, lda ),
                   lda );
#endif
      }
      if( curr != 0 ) { ii = iip1; iip1++; m = Mm1; Mm1--; }

      Nm1--; jj++;
   }
/*
 * Swap and broadcast last row - Scale last column by its absolute value
 * max entry
 */ 
   HPL_pdmxswp(  PANEL, m, ii, jj, WORK );
   HPL_dlocswpT( PANEL,    ii, jj, WORK );
   if( WORK[0] != HPL_rzero )
      HPL_dscal( Mm1, HPL_rone / WORK[0], Mptr( A, iip1, jj, lda ), 1 );
#ifdef HPL_CALL_VSIPL
/*
 * Release the blocks
 */
   (void) vsip_blockrelease_d( vsip_mgetblock_d( Yv0 ), VSIP_TRUE );
   (void) vsip_blockrelease_d( vsip_mgetblock_d( Av0 ), VSIP_TRUE );
/*
 * Destroy the matrix views
 */
   (void) vsip_mdestroy_d( Yv0 );
   (void) vsip_mdestroy_d( Av0 );
#endif
#ifdef HPL_DETAILED_TIMING
   HPL_ptimer( HPL_TIMING_PFACT );
#endif
/*
 * End of HPL_pdpanrlT
 */
}
