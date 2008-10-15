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
void HPL_dlocmax
(
   HPL_T_panel *                    PANEL,
   const int                        N,
   const int                        II,
   const int                        JJ,
   double *                         WORK
)
#else
void HPL_dlocmax
( PANEL, N, II, JJ, WORK )
   HPL_T_panel *                    PANEL;
   const int                        N;
   const int                        II;
   const int                        JJ;
   double *                         WORK;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_dlocmax finds  the maximum entry in the current column  and packs
 * the useful information in  WORK[0:3].  On exit,  WORK[0] contains the
 * local maximum  absolute value  scalar,  WORK[1] is the  corresponding
 * local row index,  WORK[2]  is the corresponding global row index, and
 * WORK[3] is the coordinate of the process owning this max.  When N  is
 * less than 1, the WORK[0:2] is initialized to zero, and WORK[3] is set
 * to the total number of process rows.
 *
 * Arguments
 * =========
 *
 * PANEL   (local input/output)          HPL_T_panel *
 *         On entry,  PANEL  points to the data structure containing the
 *         panel information.
 *
 * N       (local input)                 const int
 *         On entry,  N specifies the local number of rows of the column
 *         of A on which we operate.
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
 *         On entry, WORK  is  a workarray of size at least 4.  On exit,
 *         WORK[0] contains  the  local  maximum  absolute value scalar,
 *         WORK[1] contains  the corresponding local row index,  WORK[2]
 *         contains the corresponding global row index, and  WORK[3]  is
 *         the coordinate of process owning this max.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   double                     * A;
   int                        kk, igindx, ilindx, myrow, nb, nprow;
/* ..
 * .. Executable Statements ..
 */
   if( N > 0 )
   {
      A      = Mptr( PANEL->A, II, JJ, PANEL->lda );
      myrow  = PANEL->grid->myrow;
      nprow  = PANEL->grid->nprow;
      nb     = PANEL->nb;
      kk     = PANEL->ii + II + ( ilindx = HPL_idamax( N, A, 1 ) );
      Mindxl2g( igindx, kk, nb, nb, myrow, 0, nprow );
/*
 * WORK[0] := local maximum absolute value scalar,
 * WORK[1] := corresponding local  row index,
 * WORK[2] := corresponding global row index,
 * WORK[3] := coordinate of process owning this max.
 */
      WORK[0] = A[ilindx];         WORK[1] = (double)(ilindx);
      WORK[2] = (double)(igindx);  WORK[3] = (double)(myrow);
   }
   else
   {
/*
 * If I do not have any row of A, then set the coordinate of the process
 * (WORK[3]) owning this "ghost" row,  such that it  will never be used,
 * even if there are only zeros in the current column of A.
 */
      WORK[0] = WORK[1] = WORK[2] = HPL_rzero;
      WORK[3] = (double)(PANEL->grid->nprow);
   }
/*
 * End of HPL_dlocmax
 */
}
