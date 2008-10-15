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
void HPL_pdlaprnt
(
   const HPL_T_grid *               GRID,
   const int                        M,
   const int                        N,
   const int                        NB,
   double *                         A,
   const int                        LDA,
   const int                        IAROW,
   const int                        IACOL,
   const char *                     CMATNM
)
#else
void HPL_pdlaprnt
( GRID, M, N, NB, A, LDA, IAROW, IACOL, CMATNM )
   const HPL_T_grid *               GRID;
   const int                        M;
   const int                        N;
   const int                        NB;
   double *                         A;
   const int                        LDA;
   const int                        IAROW;
   const int                        IACOL;
   const char *                     CMATNM;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_pdlaprnt prints  to  standard  error a distributed matrix A. The
 * local pieces of  A  are sent to the process of coordinates  (0,0)  in
 * the grid and then printed.
 *
 * Arguments
 * =========
 *
 * GRID    (local input)                 const HPL_T_grid *
 *         On entry,  GRID  points  to the data structure containing the
 *         process grid information.
 *
 * M       (global input)                const int
 *         On entry,  M  specifies the number of rows of the coefficient
 *         matrix A. M must be at least zero.
 *
 * N       (global input)                const int
 *         On  entry,   N   specifies  the  number  of  columns  of  the
 *         coefficient matrix A. N must be at least zero.
 *
 * NB      (global input)                const int
 *         On entry,  NB specifies the blocking factor used to partition
 *         and distribute the matrix. NB must be larger than one.
 *
 * A       (local input)                 double *
 *         On entry,  A  points to an  array of dimension (LDA,LocQ(N)).
 *         This array contains the coefficient matrix to be printed.
 *
 * LDA     (local input)                 const int
 *         On entry, LDA specifies the leading dimension of the array A.
 *         LDA must be at least max(1,LocP(M)).
 *
 * IAROW   (global input)                const int
 *         On entry,  IAROW  specifies the row process coordinate owning
 *         the  first row of A.  IAROW  must be  larger than or equal to
 *         zero and less than NPROW.
 *
 * IACOL   (global input)                const int
 *         On entry,  IACOL  specifies  the  column  process  coordinate
 *         owning the  first column  of A. IACOL  must be larger than or
 *         equal to zero and less than NPCOL.
 *
 * CMATNM  (global input)                const char *
 *         On entry, CMATNM is the name of the matrix to be printed.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   MPI_Comm                   Acomm;
   double                     * buf = NULL;
   int                        h, i, ib, icurcol=IACOL, icurrow=IAROW,
                              ii=0, j, jb, jj=0, mycol, myrow, npcol,
                              nprow, src;
/* ..
 * .. Executable Statements ..
 */
   (void) HPL_grid_info( GRID, &nprow, &npcol, &myrow, &mycol );
   Acomm = GRID->all_comm; 
   if( ( myrow == 0 ) && ( mycol == 0 ) )
      buf = (double*)malloc( NB * sizeof( double ) );

   for( j = 0; j < N; j += NB )
   {
      jb = N-j; jb = Mmin( jb, NB );
      for( h = 0; h < jb; h++ )
      {
         (void) HPL_barrier( Acomm );

         for( i = 0; i < M; i += NB )
         {
            ib = M-i; ib = Mmin( ib, NB );
            if( ( icurrow == 0 ) && ( icurcol == 0 ) )
            {
               if( ( myrow == 0 ) && ( mycol == 0 ) )
                  HPL_dlaprnt( ib, 1, Mptr( A, ii, jj+h, LDA ), i+1,
                               j+h+1, LDA, CMATNM );
            }
            else
            {
               if( ( myrow == icurrow ) && ( mycol == icurcol ) )
               {
                  (void) HPL_send( Mptr( A, ii, jj+h, LDA ), ib, 0,
                                   9000+(j+h)*M+i, Acomm );
               }
               else if( ( myrow == 0 ) && ( mycol == 0 ) )
               {
                  src = HPL_pnum( GRID, icurrow, icurcol );
                  (void) HPL_recv( buf, ib, src, 9000+(j+h)*M+i,
                                   Acomm );
                  HPL_dlaprnt( ib, 1, buf, i+1, j+h+1, NB, CMATNM );
               }
            }
            if( myrow == icurrow ) ii += ib;
            icurrow = MModAdd1( icurrow, nprow );
            (void) HPL_barrier( Acomm );
         }
         ii = 0; icurrow = IAROW;
      }
      if( mycol == icurcol ) jj += jb;
      icurcol = MModAdd1( icurcol, npcol );
      (void) HPL_barrier( Acomm );
   }
   if( ( myrow == 0 ) && ( mycol == 0 ) && ( buf ) ) free( buf );
/*
 * End of HPL_pdlaprnt
 */
}
