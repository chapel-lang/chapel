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

#include <hpccmema.h>

#ifdef HPL_STDC_HEADERS
void HPL_pdtest
(
   HPL_T_test *                     TEST,
   HPL_T_grid *                     GRID,
   HPL_T_palg *                     ALGO,
   const int                        N,
   const int                        NB,
   HPL_RuntimeData *                rdata
)
#else
void HPL_pdtest
( TEST, GRID, ALGO, N, NB, rdata )
   HPL_T_test *                     TEST;
   HPL_T_grid *                     GRID;
   HPL_T_palg *                     ALGO;
   const int                        N;
   const int                        NB;
   HPL_RuntimeData *                rdata;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_pdtest performs  one  test  given a set of parameters such as the
 * process grid, the  problem size, the distribution blocking factor ...
 * This function generates  the data, calls  and times the linear system
 * solver,  checks  the  accuracy  of the  obtained vector solution  and
 * writes this information to the file pointed to by TEST->outfp.
 *
 * Arguments
 * =========
 *
 * TEST    (global input)                HPL_T_test *
 *         On entry,  TEST  points  to a testing data structure:  outfp
 *         specifies the output file where the results will be printed.
 *         It is only defined and used by the process  0  of the  grid.
 *         thrsh  specifies  the  threshhold value  for the test ratio.
 *         Concretely, a test is declared "PASSED"  if and only if  all
 *         of the following inequalities are satisfied:
 *         ||Ax-b||_oo / ( epsil * ||A||_1  * N            ) < thrsh,
 *         ||Ax-b||_oo / ( epsil * ||A||_1  * ||x||_1      ) < thrsh,
 *         ||Ax-b||_oo / ( epsil * ||A||_oo * ||x||_oo * N ) < thrsh.
 *         epsil  is the  relative machine precision of the distributed
 *         computer. Finally the test counters, kfail, kpass, kskip and
 *         ktest are updated as follows:  if the test passes,  kpass is
 *         incremented by one;  if the test fails, kfail is incremented
 *         by one; if the test is skipped, kskip is incremented by one.
 *         ktest is left unchanged.
 *
 * GRID    (local input)                 HPL_T_grid *
 *         On entry,  GRID  points  to the data structure containing the
 *         process grid information.
 *
 * ALGO    (global input)                HPL_T_palg *
 *         On entry,  ALGO  points to  the data structure containing the
 *         algorithmic parameters to be used for this test.
 *
 * N       (global input)                const int
 *         On entry,  N specifies the order of the coefficient matrix A.
 *         N must be at least zero.
 *
 * NB      (global input)                const int
 *         On entry,  NB specifies the blocking factor used to partition
 *         and distribute the matrix A. NB must be larger than one.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
#ifdef HPL_DETAILED_TIMING
   double                     HPL_w[HPL_TIMING_N];
#endif
   HPL_T_pmat                 mat;
   double                     wtime[1];
   int                        info[3];
   double                     Anorm1, AnormI, Gflops, Xnorm1, XnormI,
                              resid0, resid1, resid2, resid3;
   double                     * Bptr;
   void                       * vptr = NULL;
   static int                 first=1;
   int                        ii, ip2, mycol, myrow, npcol, nprow, nq;
   char                       ctop, cpfact, crfact;
/* ..
 * .. Executable Statements ..
 */
   (void) HPL_grid_info( GRID, &nprow, &npcol, &myrow, &mycol );

   mat.n  = N; mat.nb = NB; mat.info = 0;
   mat.mp = HPL_numroc( N, NB, NB, myrow, 0, nprow );
   nq     = HPL_numroc( N, NB, NB, mycol, 0, npcol );
   mat.nq = nq + 1;
/*
 * Allocate matrix, right-hand-side, and vector solution x. [ A | b ] is
 * N by N+1.  One column is added in every process column for the solve.
 * The  result  however  is stored in a 1 x N vector replicated in every
 * process row. In every process, A is lda * (nq+1), x is 1 * nq and the
 * workspace is mp. 
 *
 * Ensure that lda is a multiple of ALIGN and not a power of 2
 */
   mat.ld = ( ( Mmax( 1, mat.mp ) - 1 ) / ALGO->align ) * ALGO->align;
   do
   {
      ii = ( mat.ld += ALGO->align ); ip2 = 1;
      while( ii > 1 ) { ii >>= 1; ip2 <<= 1; }
   }
   while( mat.ld == ip2 );

/*
 * Allocate dynamic memory
 */
   vptr = (void*)HPCC_malloc( (ALGO->align + (mat.ld+1)*(mat.nq))*sizeof(double) );
   info[0] = (vptr == NULL); info[1] = myrow; info[2] = mycol;
   (void) HPL_all_reduce( (void *)(info), 3, HPL_INT, HPL_max,
                          GRID->all_comm );
   if( info[0] != 0 )
   {
      if( ( myrow == 0 ) && ( mycol == 0 ) )
         HPL_pwarn( TEST->outfp, __LINE__, "HPL_pdtest",
                    "[%d,%d] %s", info[1], info[2],
                    "Memory allocation failed for A, x and b. Skip." );
      (TEST->kskip)++;
      return;
   }
/*
 * generate matrix and right-hand-side, [ A | b ] which is N by N+1.
 */
   mat.A  = (double *)HPL_PTR( vptr,
                               ((size_t)(ALGO->align) * sizeof(double) ) );
   mat.X  = mat.A + (mat.ld * mat.nq);
   HPL_pdmatgen( GRID, N, N+1, NB, mat.A, mat.ld, HPL_ISEED );
#ifdef HPL_CALL_VSIPL
   mat.block = vsip_blockbind_d( (vsip_scalar_d *)(mat.A),
                                 (vsip_length)(mat.ld * mat.nq),
                                 VSIP_MEM_NONE );
#endif
/*
 * Solve linear system
 */
   HPL_ptimer_boot(); (void) HPL_barrier( GRID->all_comm );
   HPL_ptimer( 0 );
   HPL_pdgesv( GRID, ALGO, &mat );
   HPL_ptimer( 0 );
#ifdef HPL_CALL_VSIPL
   (void) vsip_blockrelease_d( mat.block, VSIP_TRUE ); 
   vsip_blockdestroy_d( mat.block );
#endif
/*
 * Gather max of all CPU and WALL clock timings and print timing results
 */
   HPL_ptimer_combine( GRID->all_comm, HPL_AMAX_PTIME, HPL_WALL_PTIME,
                       1, 0, wtime );

   if( ( myrow == 0 ) && ( mycol == 0 ) )
   {
      if( first )
      {
         HPL_fprintf( TEST->outfp, "%s%s\n",
                      "======================================",
                      "======================================" );
         HPL_fprintf( TEST->outfp, "%s%s\n",
                      "T/V                N    NB     P     Q",
                      "               Time             Gflops" );
         HPL_fprintf( TEST->outfp, "%s%s\n",
                      "--------------------------------------",
                      "--------------------------------------" );
         if( TEST->thrsh <= HPL_rzero ) first = 0;
      }
/*
 * 2/3 N^3 - 1/2 N^2 flops for LU factorization + 2 N^2 flops for solve.
 * Print WALL time
 */
      rdata->Gflops =
      Gflops = ( ( (double)(N) /   1.0e+9 ) * 
                 ( (double)(N) / wtime[0] ) ) * 
                 ( ( 2.0 / 3.0 ) * (double)(N) + ( 3.0 / 2.0 ) );

      rdata->cpfact =
      cpfact = ( ( ALGO->pfact == HPL_LEFT_LOOKING ) ? 'L' :
                 ( ( ALGO->pfact == HPL_CROUT ) ? 'C' : 'R' ) );
      rdata->crfact =
      crfact = ( ( ALGO->rfact == HPL_LEFT_LOOKING ) ? 'L' :
                 ( ( ALGO->rfact == HPL_CROUT ) ? 'C' : 'R' ) );

      if(      ALGO->btopo == HPL_1RING   ) ctop = '0';
      else if( ALGO->btopo == HPL_1RING_M ) ctop = '1';
      else if( ALGO->btopo == HPL_2RING   ) ctop = '2';
      else if( ALGO->btopo == HPL_2RING_M ) ctop = '3';
      else if( ALGO->btopo == HPL_BLONG   ) ctop = '4';
      else /* if( ALGO->btopo == HPL_BLONG_M ) */ ctop = '5';
      rdata->ctop = ctop;

      rdata->eps = TEST->epsil;
      rdata->order = ( GRID->order == HPL_ROW_MAJOR ? 'R' : 'C' );
      rdata->depth = ALGO->depth;
      rdata->nbdiv = ALGO->nbdiv;
      rdata->nbmin = ALGO->nbmin;
      rdata->time = wtime[0];
      rdata->N = N;
      rdata->NB = NB;
      rdata->nprow = nprow;
      rdata->npcol = npcol;
      if( wtime[0] > HPL_rzero )
         HPL_fprintf( TEST->outfp,
             "W%c%1d%c%c%1d%c%1d%12d %5d %5d %5d %18.2f %18.3e\n",
             ( GRID->order == HPL_ROW_MAJOR ? 'R' : 'C' ),
             ALGO->depth, ctop, crfact, ALGO->nbdiv, cpfact, ALGO->nbmin,
             N, NB, nprow, npcol, wtime[0], Gflops );
   }
#ifdef HPL_DETAILED_TIMING
   HPL_ptimer_combine( GRID->all_comm, HPL_AMAX_PTIME, HPL_WALL_PTIME,
                       HPL_TIMING_N, HPL_TIMING_BEG, HPL_w );
   if( ( myrow == 0 ) && ( mycol == 0 ) )
   {
      HPL_fprintf( TEST->outfp, "%s%s\n",
                   "--VVV--VVV--VVV--VVV--VVV--VVV--VVV--V",
                   "VV--VVV--VVV--VVV--VVV--VVV--VVV--VVV-" );
/*
 * Recursive panel factorization
 */
      if( HPL_w[HPL_TIMING_RPFACT-HPL_TIMING_BEG] > HPL_rzero )
         HPL_fprintf( TEST->outfp,
                      "Max aggregated wall time rfact . . . : %18.2f\n",
                      HPL_w[HPL_TIMING_RPFACT-HPL_TIMING_BEG] );
/*
 * Panel factorization
 */
      if( HPL_w[HPL_TIMING_PFACT-HPL_TIMING_BEG] > HPL_rzero )
         HPL_fprintf( TEST->outfp,
                      "+ Max aggregated wall time pfact . . : %18.2f\n",
                      HPL_w[HPL_TIMING_PFACT-HPL_TIMING_BEG] );
/*
 * Panel factorization (swap)
 */
      if( HPL_w[HPL_TIMING_MXSWP-HPL_TIMING_BEG] > HPL_rzero )
         HPL_fprintf( TEST->outfp,
                      "+ Max aggregated wall time mxswp . . : %18.2f\n",
                      HPL_w[HPL_TIMING_MXSWP-HPL_TIMING_BEG] );
/*
 * Update
 */
      if( HPL_w[HPL_TIMING_UPDATE-HPL_TIMING_BEG] > HPL_rzero )
         HPL_fprintf( TEST->outfp,
                      "Max aggregated wall time update  . . : %18.2f\n",
                      HPL_w[HPL_TIMING_UPDATE-HPL_TIMING_BEG] );
/*
 * Update (swap)
 */
      if( HPL_w[HPL_TIMING_LASWP-HPL_TIMING_BEG] > HPL_rzero )
         HPL_fprintf( TEST->outfp,
                      "+ Max aggregated wall time laswp . . : %18.2f\n",
                      HPL_w[HPL_TIMING_LASWP-HPL_TIMING_BEG] );
/*
 * Upper triangular system solve
 */
      if( HPL_w[HPL_TIMING_PTRSV-HPL_TIMING_BEG] > HPL_rzero )
         HPL_fprintf( TEST->outfp,
                      "Max aggregated wall time up tr sv  . : %18.2f\n",
                      HPL_w[HPL_TIMING_PTRSV-HPL_TIMING_BEG] );

      if( TEST->thrsh <= HPL_rzero )
         HPL_fprintf( TEST->outfp, "%s%s\n",
                      "======================================",
                      "======================================" );
   }
#endif
/*
 * Quick return, if I am not interested in checking the computations
 */
   if( TEST->thrsh <= HPL_rzero )
   { (TEST->kpass)++; if( vptr ) HPCC_free( vptr ); return; }
/*
 * Check info returned by solve
 */
   if( mat.info != 0 )
   {
      if( ( myrow == 0 ) && ( mycol == 0 ) )
         HPL_pwarn( TEST->outfp, __LINE__, "HPL_pdtest", "%s %d, %s", 
                    "Error code returned by solve is", mat.info, "skip" );
      (TEST->kskip)++;
      if( vptr ) HPCC_free( vptr ); return;
   }
/*
 * Check computation, re-generate [ A | b ], compute norm 1 and inf of A and x,
 * and norm inf of b - A x. Display residual checks.
 */
   HPL_pdmatgen( GRID, N, N+1, NB, mat.A, mat.ld, HPL_ISEED );
   rdata->Anorm1 =
   Anorm1 = HPL_pdlange( GRID, HPL_NORM_1, N, N, NB, mat.A, mat.ld );
   rdata->AnormI =
   AnormI = HPL_pdlange( GRID, HPL_NORM_I, N, N, NB, mat.A, mat.ld );
/*
 * Because x is distributed in process rows, switch the norms
 */
   rdata->XnormI =
   XnormI = HPL_pdlange( GRID, HPL_NORM_1, 1, N, NB, mat.X, 1 );
   rdata->Xnorm1 =
   Xnorm1 = HPL_pdlange( GRID, HPL_NORM_I, 1, N, NB, mat.X, 1 );
/*
 * If I own b, compute ( b - A x ) and ( - A x ) otherwise
 */
   Bptr = mat.A + mat.ld * nq;

   if( mycol == HPL_indxg2p( N, NB, NB, 0, npcol ) )
   {
      HPL_dgemv( HplColumnMajor, HplNoTrans, mat.mp, nq, -HPL_rone,
                 mat.A, mat.ld, mat.X, 1, HPL_rone, Bptr, 1 );
   }
   else if( nq > 0 )
   {
      HPL_dgemv( HplColumnMajor, HplNoTrans, mat.mp, nq, -HPL_rone,
                 mat.A, mat.ld, mat.X, 1, HPL_rzero, Bptr, 1 );
   }
   else { for( ii = 0; ii < mat.mp; ii++ ) Bptr[ii] = HPL_rzero; }
/*
 * Reduce the distributed residual in process column 0
 */
   if( mat.mp > 0 )
      (void) HPL_reduce( Bptr, mat.mp, HPL_DOUBLE, HPL_sum, 0,
                         GRID->row_comm );
/*
 * Compute || b - A x ||_oo
 */
   rdata->RnormI =
   resid0 = HPL_pdlange( GRID, HPL_NORM_I, N, 1, NB, Bptr, mat.ld );
/*
 * Computes and displays norms, residuals ...
 */
   if( ( Anorm1 == HPL_rzero ) || ( AnormI == HPL_rzero ) ||
       ( Xnorm1 == HPL_rzero ) || ( Xnorm1 == HPL_rzero ) || ( N <= 0 ) )
   {
      resid1 = resid2 = resid3 = HPL_rzero;
   }
   else
   {
      resid1 = resid0 / ( TEST->epsil * Anorm1          * (double)(N) );
      resid2 = resid0 / ( TEST->epsil * Anorm1 * Xnorm1               );
      resid3 = resid0 / ( TEST->epsil * AnormI * XnormI * (double)(N) );
   }

   if( ( Mmax( resid1, resid2 ) < TEST->thrsh ) &&
       ( resid3 < TEST->thrsh ) ) (TEST->kpass)++;
   else                           (TEST->kfail)++;

   if( ( myrow == 0 ) && ( mycol == 0 ) )
   {
      HPL_fprintf( TEST->outfp, "%s%s\n",
                   "--------------------------------------",
                   "--------------------------------------" );
      HPL_fprintf( TEST->outfp, "%s%16.7f%s%s\n",
         "||Ax-b||_oo / ( eps * ||A||_1  * N            ) = ", resid1,
         " ...... ", ( resid1 < TEST->thrsh ? "PASSED" : "FAILED" ) );
      HPL_fprintf( TEST->outfp, "%s%16.7f%s%s\n",
         "||Ax-b||_oo / ( eps * ||A||_1  * ||x||_1      ) = ", resid2,
         " ...... ", ( resid2 < TEST->thrsh ? "PASSED" : "FAILED" ) );
      HPL_fprintf( TEST->outfp, "%s%16.7f%s%s\n",
         "||Ax-b||_oo / ( eps * ||A||_oo * ||x||_oo * N ) = ", resid3,
         " ...... ", ( resid3 < TEST->thrsh ? "PASSED" : "FAILED" ) );

      if( ( resid1 >= TEST->thrsh ) || ( resid2 >= TEST->thrsh ) ||
          ( resid3 >= TEST->thrsh ) )
      {
         HPL_fprintf( TEST->outfp, "%s%18.6f\n",
         "||Ax-b||_oo  . . . . . . . . . . . . . . . . . = ", resid0 );
         HPL_fprintf( TEST->outfp, "%s%18.6f\n",
         "||A||_oo . . . . . . . . . . . . . . . . . . . = ", AnormI );
         HPL_fprintf( TEST->outfp, "%s%18.6f\n",
         "||A||_1  . . . . . . . . . . . . . . . . . . . = ", Anorm1 );
         HPL_fprintf( TEST->outfp, "%s%18.6f\n",
         "||x||_oo . . . . . . . . . . . . . . . . . . . = ", XnormI );
         HPL_fprintf( TEST->outfp, "%s%18.6f\n",
         "||x||_1  . . . . . . . . . . . . . . . . . . . = ", Xnorm1 );
      }
   }
   if( vptr ) HPCC_free( vptr );
/*
 * End of HPL_pdtest
 */
}
