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
double HPL_pdlange
(
   const HPL_T_grid *               GRID,
   const HPL_T_NORM                 NORM,
   const int                        M,
   const int                        N,
   const int                        NB,
   const double *                   A,
   const int                        LDA
)
#else
double HPL_pdlange
( GRID, NORM, M, N, NB, A, LDA )
   const HPL_T_grid *               GRID;
   const HPL_T_NORM                 NORM;
   const int                        M;
   const int                        N;
   const int                        NB;
   const double *                   A;
   const int                        LDA;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_pdlange returns  the value of the one norm,  or the infinity norm,
 * or the element of largest absolute value of a distributed matrix A:  
 *  
 *  
 *    max(abs(A(i,j))) when NORM = HPL_NORM_A,                          
 *    norm1(A),        when NORM = HPL_NORM_1,                          
 *    normI(A),        when NORM = HPL_NORM_I,                          
 *  
 * where norm1 denotes the one norm of a matrix (maximum column sum) and
 * normI denotes  the infinity norm of a matrix (maximum row sum).  Note
 * that max(abs(A(i,j))) is not a matrix norm.
 *
 * Arguments
 * =========
 *
 * GRID    (local input)                 const HPL_T_grid *
 *         On entry,  GRID  points  to the data structure containing the
 *         process grid information.
 *
 * NORM    (global input)                const HPL_T_NORM
 *         On entry,  NORM  specifies  the  value to be returned by this
 *         function as described above.
 *
 * M       (global input)                const int
 *         On entry,  M  specifies  the number  of rows of the matrix A.
 *         M must be at least zero.
 *
 * N       (global input)                const int
 *         On entry,  N specifies the number of columns of the matrix A.
 *         N must be at least zero.
 *
 * NB      (global input)                const int
 *         On entry,  NB specifies the blocking factor used to partition
 *         and distribute the matrix. NB must be larger than one.
 *
 * A       (local input)                 const double *
 *         On entry,  A  points to an array of dimension  (LDA,LocQ(N)),
 *         that contains the local pieces of the distributed matrix A.
 *
 * LDA     (local input)                 const int
 *         On entry, LDA specifies the leading dimension of the array A.
 *         LDA must be at least max(1,LocP(M)).
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   double                     s, v0=HPL_rzero, * work = NULL;
   MPI_Comm                   Acomm, Ccomm, Rcomm;
   int                        ii, jj, mp, mycol, myrow, npcol, nprow,
                              nq;
/* ..
 * .. Executable Statements ..
 */
   (void) HPL_grid_info( GRID, &nprow, &npcol, &myrow, &mycol );
   Rcomm = GRID->row_comm; Ccomm = GRID->col_comm;
   Acomm = GRID->all_comm;

   Mnumroc( mp, M, NB, NB, myrow, 0, nprow );
   Mnumroc( nq, N, NB, NB, mycol, 0, npcol );

   if( Mmin( M, N ) == 0 ) { return( v0 ); }
   else if( NORM == HPL_NORM_A )
   {
/*
 * max( abs( A ) )
 */
      if( ( nq > 0 ) && ( mp > 0 ) )
      {
         for( jj = 0; jj < nq; jj++ )
         {
            for( ii = 0; ii < mp; ii++ )
            { v0 = Mmax( v0, Mabs( *A ) ); A++; }
            A += LDA - mp;
         }
      }
      (void) HPL_reduce( (void *)(&v0), 1, HPL_DOUBLE, HPL_max, 0,
                         Acomm );
   }
   else if( NORM == HPL_NORM_1 )
   {
/*
 * Find norm_1( A ).
 */
      if( nq > 0 )
      {
         work = (double*)malloc( nq * sizeof( double ) );
         if( work == NULL )
         { HPL_pabort( __LINE__, "HPL_pdlange", "Memory allocation failed" ); }

         for( jj = 0; jj < nq; jj++ )
         {
            s = HPL_rzero;
            for( ii = 0; ii < mp; ii++ ) { s += Mabs( *A ); A++; }
            work[jj] = s; A += LDA - mp;
         }
/*
 * Find sum of global matrix columns, store on row 0 of process grid
 */
         (void) HPL_reduce( (void *)(work), nq, HPL_DOUBLE, HPL_sum,
                            0, Ccomm );
/*
 * Find maximum sum of columns for 1-norm
 */
         if( myrow == 0 )
         { v0 = work[HPL_idamax( nq, work, 1 )]; v0 = Mabs( v0 ); }
         if( work ) free( work );
      }
/*
 * Find max in row 0, store result in process (0,0)
 */
      if( myrow == 0 )
         (void) HPL_reduce( (void *)(&v0), 1, HPL_DOUBLE, HPL_max, 0,
                            Rcomm );
   }
   else if( NORM == HPL_NORM_I )
   {
/*
 * Find norm_inf( A )
 */
      if( mp > 0 )
      {
         work = (double*)malloc( mp * sizeof( double ) );
         if( work == NULL )
         { HPL_pabort( __LINE__, "HPL_pdlange", "Memory allocation failed" ); }

         for( ii = 0; ii < mp; ii++ ) { work[ii] = HPL_rzero; }

         for( jj = 0; jj < nq; jj++ )
         {
            for( ii = 0; ii < mp; ii++ )
            { work[ii] += Mabs( *A ); A++; }
            A += LDA - mp;
         }
/*       
 * Find sum of global matrix rows, store on column 0 of process grid
 */      
         (void) HPL_reduce( (void *)(work), mp, HPL_DOUBLE, HPL_sum,
                            0, Rcomm );
/*       
 * Find maximum sum of rows for inf-norm
 */      
         if( mycol == 0 )
         { v0 = work[HPL_idamax( mp, work, 1 )]; v0 = Mabs( v0 ); }
         if( work ) free( work );
      }
/*
 * Find max in column 0, store result in process (0,0)
 */
      if( mycol == 0 )
         (void) HPL_reduce( (void *)(&v0), 1, HPL_DOUBLE, HPL_max,
                            0, Ccomm );
   }
/*
 * Broadcast answer to every process in the grid
 */
   (void) HPL_broadcast( (void *)(&v0), 1, HPL_DOUBLE, 0, Acomm );

   return( v0 );
/*
 * End of HPL_pdlange
 */
}
