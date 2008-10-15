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
void HPL_dlocswpT
(
   HPL_T_panel *                    PANEL,
   const int                        II,
   const int                        JJ,
   double *                         WORK
)
#else
void HPL_dlocswpT
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
 * HPL_dlocswpT performs  the local swapping operations  within a panel.
 * The lower triangular  N0-by-N0  upper block of the panel is stored in
 * transpose form.
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
   L  = Mptr( PANEL->L1, 0, JJ, n0  );
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
               A1 = Mptr( PANEL->A, II,     0, lda );
               A2 = Mptr( A1,       ilindx, 0, lda );

               for( i = 0; i < nu; i += HPL_LOCSWP_DEPTH,
                    Wmx += HPL_LOCSWP_DEPTH, Wr0 += HPL_LOCSWP_DEPTH,
                    L   += HPL_LOCSWP_DEPTH )
               {
                  L[ 0]=*A1=Wmx[ 0]; *A2=Wr0[ 0]; A1+=lda; A2+=lda;
#if ( HPL_LOCSWP_DEPTH >  1 )
                  L[ 1]=*A1=Wmx[ 1]; *A2=Wr0[ 1]; A1+=lda; A2+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH >  2 )
                  L[ 2]=*A1=Wmx[ 2]; *A2=Wr0[ 2]; A1+=lda; A2+=lda;
                  L[ 3]=*A1=Wmx[ 3]; *A2=Wr0[ 3]; A1+=lda; A2+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH >  4 )
                  L[ 4]=*A1=Wmx[ 4]; *A2=Wr0[ 4]; A1+=lda; A2+=lda;
                  L[ 5]=*A1=Wmx[ 5]; *A2=Wr0[ 5]; A1+=lda; A2+=lda;
                  L[ 6]=*A1=Wmx[ 6]; *A2=Wr0[ 6]; A1+=lda; A2+=lda;
                  L[ 7]=*A1=Wmx[ 7]; *A2=Wr0[ 7]; A1+=lda; A2+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH >  8 )
                  L[ 8]=*A1=Wmx[ 8]; *A2=Wr0[ 8]; A1+=lda; A2+=lda;
                  L[ 9]=*A1=Wmx[ 9]; *A2=Wr0[ 9]; A1+=lda; A2+=lda;
                  L[10]=*A1=Wmx[10]; *A2=Wr0[10]; A1+=lda; A2+=lda;
                  L[11]=*A1=Wmx[11]; *A2=Wr0[11]; A1+=lda; A2+=lda;
                  L[12]=*A1=Wmx[12]; *A2=Wr0[12]; A1+=lda; A2+=lda;
                  L[13]=*A1=Wmx[13]; *A2=Wr0[13]; A1+=lda; A2+=lda;
                  L[14]=*A1=Wmx[14]; *A2=Wr0[14]; A1+=lda; A2+=lda;
                  L[15]=*A1=Wmx[15]; *A2=Wr0[15]; A1+=lda; A2+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH > 16 )
                  L[16]=*A1=Wmx[16]; *A2=Wr0[16]; A1+=lda; A2+=lda;
                  L[17]=*A1=Wmx[17]; *A2=Wr0[17]; A1+=lda; A2+=lda;
                  L[18]=*A1=Wmx[18]; *A2=Wr0[18]; A1+=lda; A2+=lda;
                  L[19]=*A1=Wmx[19]; *A2=Wr0[19]; A1+=lda; A2+=lda;
                  L[20]=*A1=Wmx[20]; *A2=Wr0[20]; A1+=lda; A2+=lda;
                  L[21]=*A1=Wmx[21]; *A2=Wr0[21]; A1+=lda; A2+=lda;
                  L[22]=*A1=Wmx[22]; *A2=Wr0[22]; A1+=lda; A2+=lda;
                  L[23]=*A1=Wmx[23]; *A2=Wr0[23]; A1+=lda; A2+=lda;
                  L[24]=*A1=Wmx[24]; *A2=Wr0[24]; A1+=lda; A2+=lda;
                  L[25]=*A1=Wmx[25]; *A2=Wr0[25]; A1+=lda; A2+=lda;
                  L[26]=*A1=Wmx[26]; *A2=Wr0[26]; A1+=lda; A2+=lda;
                  L[27]=*A1=Wmx[27]; *A2=Wr0[27]; A1+=lda; A2+=lda;
                  L[28]=*A1=Wmx[28]; *A2=Wr0[28]; A1+=lda; A2+=lda;
                  L[29]=*A1=Wmx[29]; *A2=Wr0[29]; A1+=lda; A2+=lda;
                  L[30]=*A1=Wmx[30]; *A2=Wr0[30]; A1+=lda; A2+=lda;
                  L[31]=*A1=Wmx[31]; *A2=Wr0[31]; A1+=lda; A2+=lda;
#endif
               }

               for( i = 0; i < nr; i++, A1 += lda, A2 += lda )
               { L[i] = *A1 = Wmx[i]; *A2 = Wr0[i]; }
            }
            else
            {
/*
 * otherwise the current row of  A  is swapped with itself, so just copy
 * the current of A into L1.
 */
               *Mptr( PANEL->A, II, JJ, lda ) = gmax;

               for( i = 0; i < nu; i += HPL_LOCSWP_DEPTH,
                    Wmx += HPL_LOCSWP_DEPTH, L += HPL_LOCSWP_DEPTH )
               {
                  L[ 0]=Wmx[ 0];
#if ( HPL_LOCSWP_DEPTH >  1 )
                  L[ 1]=Wmx[ 1];
#endif
#if ( HPL_LOCSWP_DEPTH >  2 )
                  L[ 2]=Wmx[ 2]; L[ 3]=Wmx[ 3];
#endif
#if ( HPL_LOCSWP_DEPTH >  4 )
                  L[ 4]=Wmx[ 4]; L[ 5]=Wmx[ 5];
                  L[ 6]=Wmx[ 6]; L[ 7]=Wmx[ 7];
#endif
#if ( HPL_LOCSWP_DEPTH >  8 )
                  L[ 8]=Wmx[ 8]; L[12]=Wmx[12];
                  L[ 9]=Wmx[ 9]; L[13]=Wmx[13];
                  L[10]=Wmx[10]; L[14]=Wmx[14];
                  L[11]=Wmx[11]; L[15]=Wmx[15];
#endif
#if ( HPL_LOCSWP_DEPTH > 16 )
                  L[16]=Wmx[16]; L[20]=Wmx[20];
                  L[17]=Wmx[17]; L[21]=Wmx[21];
                  L[18]=Wmx[18]; L[22]=Wmx[22];
                  L[19]=Wmx[19]; L[23]=Wmx[23];
                  L[24]=Wmx[24]; L[28]=Wmx[28];
                  L[25]=Wmx[25]; L[29]=Wmx[29];
                  L[26]=Wmx[26]; L[30]=Wmx[30];
                  L[27]=Wmx[27]; L[31]=Wmx[31];
#endif
               }
               for( i = 0; i < nr; i++ ) { L[i] = Wmx[i]; }
            }
         }
         else
         {
/*
 * otherwise, the row to be swapped with the current row of A is in Wmx,
 * so copy Wmx into L1 and A.
 */
            A1 = Mptr( PANEL->A, II, 0, lda );

            for( i = 0; i < nu; i += HPL_LOCSWP_DEPTH,
                 Wmx += HPL_LOCSWP_DEPTH, L += HPL_LOCSWP_DEPTH )
            {
               L[ 0]=*A1=Wmx[ 0]; A1+=lda;
#if ( HPL_LOCSWP_DEPTH >  1 )
               L[ 1]=*A1=Wmx[ 1]; A1+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH >  2 )
               L[ 2]=*A1=Wmx[ 2]; A1+=lda; L[ 3]=*A1=Wmx[ 3]; A1+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH >  4 )
               L[ 4]=*A1=Wmx[ 4]; A1+=lda; L[ 5]=*A1=Wmx[ 5]; A1+=lda;
               L[ 6]=*A1=Wmx[ 6]; A1+=lda; L[ 7]=*A1=Wmx[ 7]; A1+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH >  8 )
               L[ 8]=*A1=Wmx[ 8]; A1+=lda; L[ 9]=*A1=Wmx[ 9]; A1+=lda;
               L[10]=*A1=Wmx[10]; A1+=lda; L[11]=*A1=Wmx[11]; A1+=lda;
               L[12]=*A1=Wmx[12]; A1+=lda; L[13]=*A1=Wmx[13]; A1+=lda;
               L[14]=*A1=Wmx[14]; A1+=lda; L[15]=*A1=Wmx[15]; A1+=lda;
#endif
#if ( HPL_LOCSWP_DEPTH > 16 )
               L[16]=*A1=Wmx[16]; A1+=lda; L[17]=*A1=Wmx[17]; A1+=lda;
               L[18]=*A1=Wmx[18]; A1+=lda; L[19]=*A1=Wmx[19]; A1+=lda;
               L[20]=*A1=Wmx[20]; A1+=lda; L[21]=*A1=Wmx[21]; A1+=lda;
               L[22]=*A1=Wmx[22]; A1+=lda; L[23]=*A1=Wmx[23]; A1+=lda;
               L[24]=*A1=Wmx[24]; A1+=lda; L[25]=*A1=Wmx[25]; A1+=lda;
               L[26]=*A1=Wmx[26]; A1+=lda; L[27]=*A1=Wmx[27]; A1+=lda;
               L[28]=*A1=Wmx[28]; A1+=lda; L[29]=*A1=Wmx[29]; A1+=lda;
               L[30]=*A1=Wmx[30]; A1+=lda; L[31]=*A1=Wmx[31]; A1+=lda;
#endif
            }

            for( i = 0; i < nr; i++, A1 += lda ) { L[i]=*A1=Wmx[i]; } 
         }
      }
      else
      {
/*
 * otherwise I do not own the current row of A, so copy the max row  Wmx
 * into L1.
 */
         for( i = 0; i < nu; i += HPL_LOCSWP_DEPTH,
              Wmx += HPL_LOCSWP_DEPTH, L += HPL_LOCSWP_DEPTH )
         {
            L[ 0]=Wmx[ 0];
#if ( HPL_LOCSWP_DEPTH >  1 )
            L[ 1]=Wmx[ 1];
#endif
#if ( HPL_LOCSWP_DEPTH >  2 )
            L[ 2]=Wmx[ 2]; L[ 3]=Wmx[ 3];
#endif
#if ( HPL_LOCSWP_DEPTH >  4 )
            L[ 4]=Wmx[ 4]; L[ 5]=Wmx[ 5]; L[ 6]=Wmx[ 6]; L[ 7]=Wmx[ 7];
#endif
#if ( HPL_LOCSWP_DEPTH >  8 )
            L[ 8]=Wmx[ 8]; L[ 9]=Wmx[ 9]; L[10]=Wmx[10]; L[11]=Wmx[11];
            L[12]=Wmx[12]; L[13]=Wmx[13]; L[14]=Wmx[14]; L[15]=Wmx[15];
#endif
#if ( HPL_LOCSWP_DEPTH > 16 )
            L[16]=Wmx[16]; L[17]=Wmx[17]; L[18]=Wmx[18]; L[19]=Wmx[19];
            L[20]=Wmx[20]; L[21]=Wmx[21]; L[22]=Wmx[22]; L[23]=Wmx[23];
            L[24]=Wmx[24]; L[25]=Wmx[25]; L[26]=Wmx[26]; L[27]=Wmx[27];
            L[28]=Wmx[28]; L[29]=Wmx[29]; L[30]=Wmx[30]; L[31]=Wmx[31];
#endif
         }
         for( i = 0; i < nr; i++ ) { L[i] = Wmx[i]; }
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
           Wr0 += HPL_LOCSWP_DEPTH, L += HPL_LOCSWP_DEPTH )
      {
         L[ 0]=Wr0[ 0];
#if ( HPL_LOCSWP_DEPTH >  1 )
         L[ 1]=Wr0[ 1];
#endif
#if ( HPL_LOCSWP_DEPTH >  2 )
         L[ 2]=Wr0[ 2]; L[ 3]=Wr0[ 3];
#endif
#if ( HPL_LOCSWP_DEPTH >  4 )
         L[ 4]=Wr0[ 4]; L[ 5]=Wr0[ 5]; L[ 6]=Wr0[ 6]; L[ 7]=Wr0[ 7];
#endif
#if ( HPL_LOCSWP_DEPTH >  8 )
         L[ 8]=Wr0[ 8]; L[12]=Wr0[12]; L[ 9]=Wr0[ 9]; L[13]=Wr0[13];
         L[10]=Wr0[10]; L[14]=Wr0[14]; L[11]=Wr0[11]; L[15]=Wr0[15];
#endif
#if ( HPL_LOCSWP_DEPTH > 16 )
         L[16]=Wr0[16]; L[20]=Wr0[20]; L[17]=Wr0[17]; L[21]=Wr0[21];
         L[18]=Wr0[18]; L[22]=Wr0[22]; L[19]=Wr0[19]; L[23]=Wr0[23];
         L[24]=Wr0[24]; L[28]=Wr0[28]; L[25]=Wr0[25]; L[29]=Wr0[29];
         L[26]=Wr0[26]; L[30]=Wr0[30]; L[27]=Wr0[27]; L[31]=Wr0[31];
#endif
      }
      for( i = 0; i < nr; i++ ) { L[i] = Wr0[i]; }
/*
 * Set INFO.
 */
      if( *(PANEL->DINFO) == 0.0 )
         *(PANEL->DINFO) = (double)(PANEL->ia + JJ + 1);
   }
/*
 * End of HPL_dlocswpT
 */
}
