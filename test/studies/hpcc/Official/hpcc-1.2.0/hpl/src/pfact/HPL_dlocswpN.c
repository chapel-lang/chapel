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
/*
 * Define default value for unrolling factor
 */
#ifndef HPL_LOCSWP_DEPTH
#define    HPL_LOCSWP_DEPTH        32
#define    HPL_LOCSWP_LOG2_DEPTH    5
#endif

#ifdef HPL_STDC_HEADERS
void HPL_dlocswpN
(
   HPL_T_panel *                    PANEL,
   const int                        II,
   const int                        JJ,
   double *                         WORK
)
#else
void HPL_dlocswpN
( PANEL, II, JJ, WORK )
   HPL_T_panel *                    PANEL;
   const int                        II;
   const int                        JJ;
   double *                         WORK;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_dlocswpN performs  the local swapping operations  within a panel.
 * The lower triangular  N0-by-N0  upper block of the panel is stored in
 * no-transpose form (i.e. just like the input matrix itself).
 *
 * Arguments
 * =========
 *
 * PANEL   (local input/output)          HPL_T_panel *
 *         On entry,  PANEL  points to the data structure containing the
 *         panel information.
 *
 * II      (local input)                 const int
 *         On entry, II  specifies the row offset where the column to be
 *         operated on starts with respect to the panel.
 *
 * JJ      (local input)                 const int
 *         On entry, JJ  specifies the column offset where the column to
 *         be operated on starts with respect to the panel.
 *
 * WORK    (local workspace)             double *
 *         On entry, WORK  is a workarray of size at least 2 * (4+2*N0).
 *         WORK[0] contains  the  local  maximum  absolute value scalar,
 *         WORK[1] contains  the corresponding local row index,  WORK[2]
 *         contains the corresponding global row index, and  WORK[3]  is
 *         the coordinate of process owning this max.  The N0 length max
 *         row is stored in WORK[4:4+N0-1];  Note  that this is also the
 *         JJth row  (or column) of L1. The remaining part of this array
 *         is used as workspace.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   double                     gmax;
   double                     * A1, * A2, * L, * Wr0, * Wmx;
   int                        ilindx, lda, myrow, n0, nr, nu;
   register int               i;
/* ..
 * .. Executable Statements ..
 */
   myrow = PANEL->grid->myrow; n0 = PANEL->jb; lda = PANEL->lda;

   Wr0   = ( Wmx = WORK + 4 ) + n0; Wmx[JJ] = gmax = WORK[0];
   nu    = (int)( ( (unsigned int)(n0) >> HPL_LOCSWP_LOG2_DEPTH ) 
                  << HPL_LOCSWP_LOG2_DEPTH );
   nr    = n0 - nu;
/*
 * Replicated swap and copy of the current (new) row of A into L1
 */
   L  = Mptr( PANEL->L1, JJ, 0, n0  );
/*
 * If the pivot is non-zero ...
 */
   if( gmax != HPL_rzero )
   {
/*
 * and if I own the current row of A ...
 */
      if( myrow == PANEL->prow )
      {
/*
 * and if I also own the row to be swapped with the current row of A ...
 */
         if( myrow == (int)(WORK[3]) )
         {
/*
 * and if the current row of A is not to swapped with itself ...
 */
            if( ( ilindx = (int)(WORK[1]) ) != 0 )
            {
/*
 * then copy the max row into L1 and locally swap the 2 rows of A.
 */
               A1 = Mptr( PANEL->A,  II,     0, lda );
               A2 = Mptr( A1,        ilindx, 0, lda );

               for( i = 0; i < nu; i += HPL_LOCSWP_DEPTH,
                    Wmx += HPL_LOCSWP_DEPTH, Wr0 += HPL_LOCSWP_DEPTH )
               {
                  *L=*A1=Wmx[ 0]; *A2=Wr0[ 0]; L+=n0; A1+=lda; A2+=lda;
#if ( HPL_LOCSWP_DEPTH >  1 )
                  *L=*A1=Wmx[ 1]; *A2=Wr0[ 1]; L+=n0; A1+=lda; A2+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH >  2 )
                  *L=*A1=Wmx[ 2]; *A2=Wr0[ 2]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[ 3]; *A2=Wr0[ 3]; L+=n0; A1+=lda; A2+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH >  4 )
                  *L=*A1=Wmx[ 4]; *A2=Wr0[ 4]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[ 5]; *A2=Wr0[ 5]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[ 6]; *A2=Wr0[ 6]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[ 7]; *A2=Wr0[ 7]; L+=n0; A1+=lda; A2+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH >  8 )
                  *L=*A1=Wmx[ 8]; *A2=Wr0[ 8]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[ 9]; *A2=Wr0[ 9]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[10]; *A2=Wr0[10]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[11]; *A2=Wr0[11]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[12]; *A2=Wr0[12]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[13]; *A2=Wr0[13]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[14]; *A2=Wr0[14]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[15]; *A2=Wr0[15]; L+=n0; A1+=lda; A2+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH > 16 )
                  *L=*A1=Wmx[16]; *A2=Wr0[16]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[17]; *A2=Wr0[17]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[18]; *A2=Wr0[18]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[19]; *A2=Wr0[19]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[20]; *A2=Wr0[20]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[21]; *A2=Wr0[21]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[22]; *A2=Wr0[22]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[23]; *A2=Wr0[23]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[24]; *A2=Wr0[24]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[25]; *A2=Wr0[25]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[26]; *A2=Wr0[26]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[27]; *A2=Wr0[27]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[28]; *A2=Wr0[28]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[29]; *A2=Wr0[29]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[30]; *A2=Wr0[30]; L+=n0; A1+=lda; A2+=lda;
                  *L=*A1=Wmx[31]; *A2=Wr0[31]; L+=n0; A1+=lda; A2+=lda;
#endif
               }
               for( i = 0; i < nr; i++, L += n0, A1 += lda, A2 += lda )
               { *L = *A1 = Wmx[i]; *A2 = Wr0[i]; }
            }
            else
            {
/*
 * otherwise the current row of  A  is swapped with itself, so just copy
 * the current of A into L1.
 */
               *Mptr( PANEL->A, II, JJ, lda ) = gmax;

               for( i = 0; i < nu; i += HPL_LOCSWP_DEPTH,
                    Wmx += HPL_LOCSWP_DEPTH, Wr0 += HPL_LOCSWP_DEPTH )
               {
                  *L = Wmx[ 0]; L+=n0;
#if ( HPL_LOCSWP_DEPTH >  1 )
                  *L = Wmx[ 1]; L+=n0;
#endif
#if ( HPL_LOCSWP_DEPTH >  2 )
                  *L = Wmx[ 2]; L+=n0; *L = Wmx[ 3]; L+=n0;
#endif
#if ( HPL_LOCSWP_DEPTH >  4 )
                  *L = Wmx[ 4]; L+=n0; *L = Wmx[ 5]; L+=n0;
                  *L = Wmx[ 6]; L+=n0; *L = Wmx[ 7]; L+=n0;
#endif
#if ( HPL_LOCSWP_DEPTH >  8 )
                  *L = Wmx[ 8]; L+=n0; *L = Wmx[ 9]; L+=n0;
                  *L = Wmx[10]; L+=n0; *L = Wmx[11]; L+=n0;
                  *L = Wmx[12]; L+=n0; *L = Wmx[13]; L+=n0;
                  *L = Wmx[14]; L+=n0; *L = Wmx[15]; L+=n0;
#endif
#if ( HPL_LOCSWP_DEPTH > 16 )
                  *L = Wmx[16]; L+=n0; *L = Wmx[17]; L+=n0;
                  *L = Wmx[18]; L+=n0; *L = Wmx[19]; L+=n0;
                  *L = Wmx[20]; L+=n0; *L = Wmx[21]; L+=n0;
                  *L = Wmx[22]; L+=n0; *L = Wmx[23]; L+=n0;
                  *L = Wmx[24]; L+=n0; *L = Wmx[25]; L+=n0;
                  *L = Wmx[26]; L+=n0; *L = Wmx[27]; L+=n0;
                  *L = Wmx[28]; L+=n0; *L = Wmx[29]; L+=n0;
                  *L = Wmx[30]; L+=n0; *L = Wmx[31]; L+=n0;
#endif
               }
               for( i = 0; i < nr; i++, L += n0 ) { *L = Wmx[i]; }
            }
         }
         else
         {
/*
 * otherwise, the row to be swapped with the current row of A is in Wmx,
 * so copy Wmx into L1 and A.
 */
            A1 = Mptr( PANEL->A,  II, 0, lda );

            for( i = 0; i < nu; i += HPL_LOCSWP_DEPTH,
                 Wmx += HPL_LOCSWP_DEPTH )
            {
               *L = *A1 = Wmx[ 0]; L += n0; A1 += lda;
#if ( HPL_LOCSWP_DEPTH >  1 )
               *L = *A1 = Wmx[ 1]; L += n0; A1 += lda;
#endif
#if ( HPL_LOCSWP_DEPTH >  2 )
               *L = *A1 = Wmx[ 2]; L += n0; A1 += lda;
               *L = *A1 = Wmx[ 3]; L += n0; A1 += lda;
#endif
#if ( HPL_LOCSWP_DEPTH >  4 )
               *L = *A1 = Wmx[ 4]; L += n0; A1 += lda;
               *L = *A1 = Wmx[ 5]; L += n0; A1 += lda;
               *L = *A1 = Wmx[ 6]; L += n0; A1 += lda;
               *L = *A1 = Wmx[ 7]; L += n0; A1 += lda;
#endif
#if ( HPL_LOCSWP_DEPTH >  8 )
               *L = *A1 = Wmx[ 8]; L += n0; A1 += lda;
               *L = *A1 = Wmx[ 9]; L += n0; A1 += lda;
               *L = *A1 = Wmx[10]; L += n0; A1 += lda;
               *L = *A1 = Wmx[11]; L += n0; A1 += lda;
               *L = *A1 = Wmx[12]; L += n0; A1 += lda;
               *L = *A1 = Wmx[13]; L += n0; A1 += lda;
               *L = *A1 = Wmx[14]; L += n0; A1 += lda;
               *L = *A1 = Wmx[15]; L += n0; A1 += lda;
#endif
#if ( HPL_LOCSWP_DEPTH > 16 )
               *L = *A1 = Wmx[16]; L += n0; A1 += lda;
               *L = *A1 = Wmx[17]; L += n0; A1 += lda;
               *L = *A1 = Wmx[18]; L += n0; A1 += lda;
               *L = *A1 = Wmx[19]; L += n0; A1 += lda;
               *L = *A1 = Wmx[20]; L += n0; A1 += lda;
               *L = *A1 = Wmx[21]; L += n0; A1 += lda;
               *L = *A1 = Wmx[22]; L += n0; A1 += lda;
               *L = *A1 = Wmx[23]; L += n0; A1 += lda;
               *L = *A1 = Wmx[24]; L += n0; A1 += lda;
               *L = *A1 = Wmx[25]; L += n0; A1 += lda;
               *L = *A1 = Wmx[26]; L += n0; A1 += lda;
               *L = *A1 = Wmx[27]; L += n0; A1 += lda;
               *L = *A1 = Wmx[28]; L += n0; A1 += lda;
               *L = *A1 = Wmx[29]; L += n0; A1 += lda;
               *L = *A1 = Wmx[30]; L += n0; A1 += lda;
               *L = *A1 = Wmx[31]; L += n0; A1 += lda;
#endif
            }

            for( i = 0; i < nr; i++, L += n0, A1 += lda )
            { *L = *A1 = Wmx[i]; }
         }
      }
      else
      {
/*
 * otherwise I do not own the current row of A, so copy the max row  Wmx
 * into L1.
 */
         for( i = 0; i < nu; i += HPL_LOCSWP_DEPTH,
              Wmx += HPL_LOCSWP_DEPTH )
         {
            *L = Wmx[ 0]; L+=n0;
#if ( HPL_LOCSWP_DEPTH >  1 )
            *L = Wmx[ 1]; L+=n0;
#endif
#if ( HPL_LOCSWP_DEPTH >  2 )
            *L = Wmx[ 2]; L+=n0; *L = Wmx[ 3]; L+=n0;
#endif
#if ( HPL_LOCSWP_DEPTH >  4 )
            *L = Wmx[ 4]; L+=n0; *L = Wmx[ 5]; L+=n0;
            *L = Wmx[ 6]; L+=n0; *L = Wmx[ 7]; L+=n0;
#endif
#if ( HPL_LOCSWP_DEPTH >  8 )
            *L = Wmx[ 8]; L+=n0; *L = Wmx[ 9]; L+=n0;
            *L = Wmx[10]; L+=n0; *L = Wmx[11]; L+=n0;
            *L = Wmx[12]; L+=n0; *L = Wmx[13]; L+=n0;
            *L = Wmx[14]; L+=n0; *L = Wmx[15]; L+=n0;
#endif
#if ( HPL_LOCSWP_DEPTH > 16 )
            *L = Wmx[16]; L+=n0; *L = Wmx[17]; L+=n0;
            *L = Wmx[18]; L+=n0; *L = Wmx[19]; L+=n0;
            *L = Wmx[20]; L+=n0; *L = Wmx[21]; L+=n0;
            *L = Wmx[22]; L+=n0; *L = Wmx[23]; L+=n0;
            *L = Wmx[24]; L+=n0; *L = Wmx[25]; L+=n0;
            *L = Wmx[26]; L+=n0; *L = Wmx[27]; L+=n0;
            *L = Wmx[28]; L+=n0; *L = Wmx[29]; L+=n0;
            *L = Wmx[30]; L+=n0; *L = Wmx[31]; L+=n0;
#endif
         }
         for( i = 0; i < nr; i++, L += n0 ) { *L = Wmx[i]; }
/*
 * and if I own the max row, overwrite it with the current row Wr0.
 */
         if( myrow == (int)(WORK[3]) )
         {
            A2 = Mptr( PANEL->A, II + (int)(WORK[1]), 0, lda );

            for( i = 0; i < nu; i += HPL_LOCSWP_DEPTH,
                 Wr0 += HPL_LOCSWP_DEPTH )
            {
               *A2 = Wr0[ 0]; A2+=lda;
#if ( HPL_LOCSWP_DEPTH >  1 )
               *A2 = Wr0[ 1]; A2+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH >  2 )
               *A2 = Wr0[ 2]; A2+=lda; *A2 = Wr0[ 3]; A2+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH >  4 )
               *A2 = Wr0[ 4]; A2+=lda; *A2 = Wr0[ 5]; A2+=lda;
               *A2 = Wr0[ 6]; A2+=lda; *A2 = Wr0[ 7]; A2+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH >  8 )
               *A2 = Wr0[ 8]; A2+=lda; *A2 = Wr0[ 9]; A2+=lda;
               *A2 = Wr0[10]; A2+=lda; *A2 = Wr0[11]; A2+=lda;
               *A2 = Wr0[12]; A2+=lda; *A2 = Wr0[13]; A2+=lda;
               *A2 = Wr0[14]; A2+=lda; *A2 = Wr0[15]; A2+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH > 16 )
               *A2 = Wr0[16]; A2+=lda; *A2 = Wr0[17]; A2+=lda;
               *A2 = Wr0[18]; A2+=lda; *A2 = Wr0[19]; A2+=lda;
               *A2 = Wr0[20]; A2+=lda; *A2 = Wr0[21]; A2+=lda;
               *A2 = Wr0[22]; A2+=lda; *A2 = Wr0[23]; A2+=lda;
               *A2 = Wr0[24]; A2+=lda; *A2 = Wr0[25]; A2+=lda;
               *A2 = Wr0[26]; A2+=lda; *A2 = Wr0[27]; A2+=lda;
               *A2 = Wr0[28]; A2+=lda; *A2 = Wr0[29]; A2+=lda;
               *A2 = Wr0[30]; A2+=lda; *A2 = Wr0[31]; A2+=lda;
#endif
            }

            for( i = 0; i < nr; i++, A2 += lda ) { *A2 = Wr0[i]; }
         }
      }
   }
   else
   {
/*
 * Otherwise the max element in the current column is zero,  simply copy
 * the current row Wr0 into L1. The matrix is singular.
 */
      for( i = 0; i < nu; i += HPL_LOCSWP_DEPTH,
           Wr0 += HPL_LOCSWP_DEPTH )
      {
         *L = Wr0[ 0]; L+=n0;
#if ( HPL_LOCSWP_DEPTH >  1 )
         *L = Wr0[ 1]; L+=n0;
#endif
#if ( HPL_LOCSWP_DEPTH >  2 )
         *L = Wr0[ 2]; L+=n0; *L = Wr0[ 3]; L+=n0;
#endif
#if ( HPL_LOCSWP_DEPTH >  4 )
         *L = Wr0[ 4]; L+=n0; *L = Wr0[ 5]; L+=n0;
         *L = Wr0[ 6]; L+=n0; *L = Wr0[ 7]; L+=n0;
#endif
#if ( HPL_LOCSWP_DEPTH >  8 )
         *L = Wr0[ 8]; L+=n0; *L = Wr0[ 9]; L+=n0;
         *L = Wr0[10]; L+=n0; *L = Wr0[11]; L+=n0;
         *L = Wr0[12]; L+=n0; *L = Wr0[13]; L+=n0;
         *L = Wr0[14]; L+=n0; *L = Wr0[15]; L+=n0;
#endif
#if ( HPL_LOCSWP_DEPTH > 16 )
         *L = Wr0[16]; L+=n0; *L = Wr0[17]; L+=n0;
         *L = Wr0[18]; L+=n0; *L = Wr0[19]; L+=n0;
         *L = Wr0[20]; L+=n0; *L = Wr0[21]; L+=n0;
         *L = Wr0[22]; L+=n0; *L = Wr0[23]; L+=n0;
         *L = Wr0[24]; L+=n0; *L = Wr0[25]; L+=n0;
         *L = Wr0[26]; L+=n0; *L = Wr0[27]; L+=n0;
         *L = Wr0[28]; L+=n0; *L = Wr0[29]; L+=n0;
         *L = Wr0[30]; L+=n0; *L = Wr0[31]; L+=n0;
#endif
      }

      for( i = 0; i < nr; i++, L += n0 ) { *L = Wr0[i]; }
/*
 * set INFO.
 */
      if( *(PANEL->DINFO) == 0.0 )
         *(PANEL->DINFO) = (double)(PANEL->ia + JJ + 1);
   }
/*
 * End of HPL_dlocswpN
 */
}
