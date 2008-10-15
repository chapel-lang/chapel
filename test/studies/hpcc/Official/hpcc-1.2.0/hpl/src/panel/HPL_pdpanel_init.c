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
void HPL_pdpanel_init
(
   HPL_T_grid *                     GRID,
   HPL_T_palg *                     ALGO,
   const int                        M,
   const int                        N,
   const int                        JB,
   HPL_T_pmat *                     A,
   const int                        IA,
   const int                        JA,
   const int                        TAG,
   HPL_T_panel *                    PANEL
)
#else
void HPL_pdpanel_init
( GRID, ALGO, M, N, JB, A, IA, JA, TAG, PANEL )
   HPL_T_grid *                     GRID;
   HPL_T_palg *                     ALGO;
   const int                        M;
   const int                        N;
   const int                        JB;
   HPL_T_pmat *                     A;
   const int                        IA;
   const int                        JA;
   const int                        TAG;
   HPL_T_panel *                    PANEL;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_pdpanel_init initializes a panel data structure.
 * 
 *
 * Arguments
 * =========
 *
 * GRID    (local input)                 HPL_T_grid *
 *         On entry,  GRID  points  to the data structure containing the
 *         process grid information.
 *
 * ALGO    (global input)                HPL_T_palg *
 *         On entry,  ALGO  points to  the data structure containing the
 *         algorithmic parameters.
 *
 * M       (local input)                 const int
 *         On entry, M specifies the global number of rows of the panel.
 *         M must be at least zero.
 *
 * N       (local input)                 const int
 *         On entry,  N  specifies  the  global number of columns of the
 *         panel and trailing submatrix. N must be at least zero.
 *
 * JB      (global input)                const int
 *         On entry, JB specifies is the number of columns of the panel.
 *         JB must be at least zero.
 *
 * A       (local input/output)          HPL_T_pmat *
 *         On entry, A points to the data structure containing the local
 *         array information.
 *
 * IA      (global input)                const int
 *         On entry,  IA  is  the global row index identifying the panel
 *         and trailing submatrix. IA must be at least zero.
 *
 * JA      (global input)                const int
 *         On entry, JA is the global column index identifying the panel
 *         and trailing submatrix. JA must be at least zero.
 *
 * TAG     (global input)                const int
 *         On entry, TAG is the row broadcast message id.
 *
 * PANEL   (local input/output)          HPL_T_panel *
 *         On entry,  PANEL  points to the data structure containing the
 *         panel information.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   size_t                     dalign;
   int                        icurcol, icurrow, ii, itmp1, jj, lwork,
                              ml2, mp, mycol, myrow, nb, npcol, nprow,
                              nq, nu;
/* ..
 * .. Executable Statements ..
 */
   PANEL->grid    = GRID;                  /* ptr to the process grid */
   PANEL->algo    = ALGO;               /* ptr to the algo parameters */
   PANEL->pmat    = A;                 /* ptr to the local array info */

   myrow = GRID->myrow; mycol = GRID->mycol;
   nprow = GRID->nprow; npcol = GRID->npcol; nb = A->nb;

   HPL_infog2l( IA, JA, nb, nb, nb, nb, 0, 0, myrow, mycol,
                nprow, npcol, &ii, &jj, &icurrow, &icurcol );
   mp = HPL_numrocI( M, IA, nb, nb, myrow, 0, nprow );
   nq = HPL_numrocI( N, JA, nb, nb, mycol, 0, npcol );
                                         /* ptr to trailing part of A */
   PANEL->A       = Mptr( (double *)(A->A), ii, jj, A->ld );
/*
 * Workspace pointers are initialized to NULL.
 */
   PANEL->WORK    = NULL; PANEL->L2      = NULL; PANEL->L1      = NULL;
   PANEL->DPIV    = NULL; PANEL->DINFO   = NULL; PANEL->U       = NULL;
   PANEL->IWORK   = NULL;
/*
 * Local lengths, indexes process coordinates
 */
   PANEL->nb      = nb;               /* distribution blocking factor */
   PANEL->jb      = JB;                                /* panel width */
   PANEL->m       = M;      /* global # of rows of trailing part of A */
   PANEL->n       = N;      /* global # of cols of trailing part of A */
   PANEL->ia      = IA;     /* global row index of trailing part of A */
   PANEL->ja      = JA;     /* global col index of trailing part of A */
   PANEL->mp      = mp;      /* local # of rows of trailing part of A */
   PANEL->nq      = nq;      /* local # of cols of trailing part of A */
   PANEL->ii      = ii;      /* local row index of trailing part of A */
   PANEL->jj      = jj;      /* local col index of trailing part of A */
   PANEL->lda     = A->ld;            /* local leading dim of array A */
   PANEL->prow    = icurrow; /* proc row owning 1st row of trailing A */
   PANEL->pcol    = icurcol; /* proc col owning 1st col of trailing A */
   PANEL->msgid   = TAG;     /* message id to be used for panel bcast */
/*
 * Initialize  ldl2 and len to temporary dummy values and Update tag for
 * next panel
 */
   PANEL->ldl2    = 0;               /* local leading dim of array L2 */
   PANEL->len     = 0;           /* length of the buffer to broadcast */
/*
 * Figure out the exact amount of workspace  needed by the factorization
 * and the update - Allocate that space - Finish the panel data structu-
 * re initialization.
 *
 * L1:    JB x JB in all processes
 * DPIV:  JB      in all processes
 * DINFO: 1       in all processes
 *
 * We make sure that those three arrays are contiguous in memory for the
 * later panel broadcast.  We  also  choose  to put this amount of space 
 * right  after  L2 (when it exist) so that one can receive a contiguous
 * buffer.
 */
   dalign = ALGO->align * sizeof( double );

   if( npcol == 1 )                             /* P x 1 process grid */
   {                                     /* space for L1, DPIV, DINFO */
      lwork = ALGO->align + ( PANEL->len = JB * JB + JB + 1 );
      if( nprow > 1 )                                 /* space for U */
      { nu = nq - JB; lwork += JB * Mmax( 0, nu ); }

      if( !( PANEL->WORK = (double *)malloc( lwork * sizeof( double ) ) ) )
      {
         HPL_pabort( __LINE__, "HPL_pdpanel_init",
                     "Memory allocation failed" );
      }
/*
 * Initialize the pointers of the panel structure  -  Always re-use A in
 * the only process column
 */
      PANEL->L2    = PANEL->A + ( myrow == icurrow ? JB : 0 );
      PANEL->ldl2  = A->ld;
      PANEL->L1    = (double *)HPL_PTR( PANEL->WORK, dalign );
      PANEL->DPIV  = PANEL->L1    + JB * JB;
      PANEL->DINFO = PANEL->DPIV + JB;       *(PANEL->DINFO) = 0.0;
      PANEL->U     = ( nprow > 1 ? PANEL->DINFO + 1: NULL );
   }
   else
   {                                        /* space for L2, L1, DPIV */
      ml2 = ( myrow == icurrow ? mp - JB : mp ); ml2 = Mmax( 0, ml2 );
      PANEL->len = ml2*JB + ( itmp1 = JB*JB + JB + 1 );
#ifdef HPL_COPY_L
      lwork = ALGO->align + PANEL->len;
#else
      lwork = ALGO->align + ( mycol == icurcol ? itmp1 : PANEL->len );
#endif
      if( nprow > 1 )                                 /* space for U */
      { 
         nu = ( mycol == icurcol ? nq - JB : nq );
         lwork += JB * Mmax( 0, nu );
      }

      if( !( PANEL->WORK = (double *)malloc( lwork * sizeof( double ) ) ) )
      {
         HPL_pabort( __LINE__, "HPL_pdpanel_init",
                     "Memory allocation failed" );
      }
/*
 * Initialize the pointers of the panel structure - Re-use A in the cur-
 * rent process column when HPL_COPY_L is not defined.
 */
#ifdef HPL_COPY_L
      PANEL->L2    = (double *)HPL_PTR( PANEL->WORK, dalign );
      PANEL->ldl2  = Mmax( 1, ml2 );
      PANEL->L1    = PANEL->L2 + ml2 * JB;
#else
      if( mycol == icurcol )
      {
         PANEL->L2   = PANEL->A + ( myrow == icurrow ? JB : 0 );
         PANEL->ldl2 = A->ld;
         PANEL->L1   = (double *)HPL_PTR( PANEL->WORK, dalign );
      }
      else
      {
         PANEL->L2   = (double *)HPL_PTR( PANEL->WORK, dalign );
         PANEL->ldl2 = Mmax( 1, ml2 );
         PANEL->L1   = PANEL->L2 + ml2 * JB;
      } 
#endif
      PANEL->DPIV  = PANEL->L1   + JB * JB;
      PANEL->DINFO = PANEL->DPIV + JB;     *(PANEL->DINFO) = 0.0;
      PANEL->U     = ( nprow > 1 ? PANEL->DINFO + 1 : NULL );
   }
#ifdef HPL_CALL_VSIPL
   PANEL->Ablock  = A->block;
/*
 * Create blocks and bind them to the data pointers
 */
   PANEL->L1block = vsip_blockbind_d( (vsip_scalar_d *)(PANEL->L1),
                                      (vsip_length)(JB*JB), VSIP_MEM_NONE );
   PANEL->L2block = vsip_blockbind_d( (vsip_scalar_d *)(PANEL->L2),
                                      (vsip_length)(PANEL->ldl2*JB),
                                      VSIP_MEM_NONE );
   if( nprow > 1 )
   { 
      nu = ( mycol == icurcol ? nq - JB : nq );
      PANEL->Ublock = vsip_blockbind_d( (vsip_scalar_d *)(PANEL->U),
                                        (vsip_length)(JB * Mmax( 0, nu )),
                                        VSIP_MEM_NONE );
   }
   else { PANEL->Ublock = A->block; }
#endif
/*
 * If nprow is 1, we just allocate an array of JB integers for the swap.
 * When nprow > 1, we allocate the space for the index arrays immediate-
 * ly. The exact size of this array depends on the swapping routine that
 * will be used, so we allocate the maximum:
 *
 *    IWORK[0] is of size at most 1      +
 *    IPL      is of size at most 1      +
 *    IPID     is of size at most 4 * JB +
 *
 *    For HPL_pdlaswp00:
 *       lindxA   is of size at most 2 * JB +
 *       lindxAU  is of size at most 2 * JB +
 *       llen     is of size at most NPROW  +
 *       llen_sv  is of size at most NPROW.
 *
 *    For HPL_pdlaswp01:
 *       ipA      is of size ar most 1      +
 *       lindxA   is of size at most 2 * JB +
 *       lindxAU  is of size at most 2 * JB +
 *       iplen    is of size at most NPROW  + 1 +
 *       ipmap    is of size at most NPROW  +
 *       ipmapm1  is of size at most NPROW  +
 *       permU    is of size at most JB     +
 *       iwork    is of size at most MAX( 2*JB, NPROW+1 ).
 *
 * that is  3 + 8*JB + MAX(2*NPROW, 3*NPROW+1+JB+MAX(2*JB,NPROW+1))
 *       =  4 + 9*JB + 3*NPROW + MAX( 2*JB, NPROW+1 ).
 *
 * We use the fist entry of this to work array  to indicate  whether the
 * the  local  index arrays have already been computed,  and if yes,  by
 * which function:
 *    IWORK[0] = -1: no index arrays have been computed so far;
 *    IWORK[0] =  0: HPL_pdlaswp00 already computed those arrays;
 *    IWORK[0] =  1: HPL_pdlaswp01 already computed those arrays;
 * This allows to save some redundant and useless computations.
 */
   if( nprow == 1 ) { lwork = JB; }
   else             
   {
      itmp1 = (JB << 1); lwork = nprow + 1; itmp1 = Mmax( itmp1, lwork );
      lwork = 4 + (9 * JB) + (3 * nprow) + itmp1;
   }

   PANEL->IWORK = (int *)malloc( lwork * sizeof( int ) );

   if( PANEL->IWORK == NULL )
   { HPL_pabort( __LINE__, "HPL_pdpanel_init", "Memory allocation failed" ); }
                       /* Initialize the first entry of the workarray */
   *(PANEL->IWORK) = -1;
/*
 * End of HPL_pdpanel_init
 */
}
