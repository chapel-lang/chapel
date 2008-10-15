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
int HPL_packL
(
   HPL_T_panel *                    PANEL,
   const int                        INDEX,
   const int                        LEN,
   const int                        IBUF
)
#else
int HPL_packL
( PANEL, INDEX, LEN, IBUF )
   HPL_T_panel *                    PANEL;
   const int                        INDEX;
   const int                        LEN;
   const int                        IBUF;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_packL forms  the MPI data type for the panel to be broadcast.
 * Successful  completion  is  indicated  by  the  returned  error  code
 * MPI_SUCCESS.
 *
 * Arguments
 * =========
 *
 * PANEL   (input/output)                HPL_T_panel *
 *         On entry,  PANEL  points to the  current panel data structure
 *         being broadcast.
 *
 * INDEX   (input)                       const int
 *         On entry,  INDEX  points  to  the  first entry of the  packed
 *         buffer being broadcast.
 *
 * LEN     (input)                       const int
 *         On entry, LEN is the length of the packed buffer.
 *
 * IBUF    (input)                       const int
 *         On entry, IBUF  specifies the panel buffer/count/type entries
 *         that should be initialized.
 *
 * ---------------------------------------------------------------------
 */ 
#ifdef HPL_USE_MPI_DATATYPE
/*
 * .. Local Variables ..
 */
#ifndef HPL_COPY_L
   MPI_Datatype               * type = NULL;
   void                       * * * bufs = NULL;
   double                     * A;
   int                        * blen = NULL;
   MPI_Aint                   * disp = NULL;
   int                        curr, i, i1, ibuf, ierr=MPI_SUCCESS, j1,
                              jb, jbm, jbp1, lda, len, m, m1, nbufs;
#else
   int                        ierr;
#endif
/* ..
 * .. Executable Statements ..
 */
#ifdef HPL_COPY_L
/*
 * Panel + L1 + DPIV  have been copied into a contiguous buffer - Create
 * and commit a contiguous data type
 */
   PANEL->buffers[IBUF] = (void *)(PANEL->L2 + INDEX);
   PANEL->counts [IBUF] = 1;

   ierr =      MPI_Type_contiguous( LEN, MPI_DOUBLE, &PANEL->dtypes[IBUF] );
   if( ierr == MPI_SUCCESS )
      ierr =   MPI_Type_commit( &PANEL->dtypes[IBUF] );

   return( ierr );
#else
/*
 * Panel is not contiguous (because of LDA and also L1 + DPIV) -  Create
 * and commit a struct data type
 */
   jbp1 = ( jb = PANEL->jb ) + 1;
/*
 * Temporaries to create the type struct.
 */
   bufs = (void     * * *)malloc( jbp1 * sizeof( void * *     ) );
   blen = (int          *)malloc( jbp1 * sizeof( int          ) );
   disp = (MPI_Aint     *)malloc( jbp1 * sizeof( MPI_Aint     ) );
   type = (MPI_Datatype *)malloc( jbp1 * sizeof( MPI_Datatype ) );
 
   if( ( bufs != NULL ) && ( blen != NULL ) &&
       ( disp != NULL ) && ( type != NULL ) )
   {
      m = PANEL->mp; curr = (int)( PANEL->grid->myrow == PANEL->prow );
      if( curr != 0 ) m -= jb;
 
      len = LEN; ibuf = INDEX; nbufs = 0; jbm = jb * m;
 
      if( ( m > 0 ) && ( ibuf < jbm ) )
      {
/*
 * Retrieve proper pointers depending on process row and column
 */
         if( PANEL->grid->mycol == PANEL->pcol )
         {
            lda = PANEL->lda;
            if( curr != 0 ) { A = Mptr( PANEL->A, jb, -jb, lda ); }
            else            { A = Mptr( PANEL->A,  0, -jb, lda ); }
         }
         else { lda = PANEL->ldl2; A = PANEL->L2; }
/*
 * Pack the first (partial) column of L
 */
         m1 = m - ( i1 = ibuf - ( j1 = ibuf / m ) * m );
         m1 = Mmin( len, m1 );
 
         bufs[nbufs] = (void **)(Mptr( A, i1, j1, lda ));
         type[nbufs] = MPI_DOUBLE;
         blen[nbufs] = m1;
         if( ierr == MPI_SUCCESS )
            ierr =   MPI_Address( bufs[nbufs], &disp[nbufs] );
 
         nbufs++; len -= m1; j1++; ibuf += m1;
/*
 * Pack the remaining columns of L
 */
         while( ( len > 0 ) && ( j1 < jb ) )
         {
            m1 = Mmin( len, m );
 
            bufs[nbufs] = (void**)(Mptr( A, 0, j1, lda ));
            type[nbufs] = MPI_DOUBLE;
            blen[nbufs] = m1;
            if( ierr == MPI_SUCCESS )
               ierr =   MPI_Address( bufs[nbufs], &disp[nbufs] );
 
            nbufs++; len -= m1; j1++; ibuf += m1;
         }
      }
/*
 * Pack L1, DPIV, DINFO
 */
      if( len > 0 )
      {                                            /* L1, DPIV, DINFO */
         bufs[nbufs] = (void **)(PANEL->L1 + ibuf - jbm);
         type[nbufs] = MPI_DOUBLE;
         blen[nbufs] = len;
         if( ierr == MPI_SUCCESS )
            ierr =   MPI_Address( bufs[nbufs], &disp[nbufs] );
         nbufs++;
      }
 
      for( i = 1; i < nbufs; i++ ) disp[i] -= disp[0]; disp[0] = 0;
 
      PANEL->buffers[IBUF] = (void ***)(bufs[0]); PANEL->counts [IBUF] = 1;
/*
 * construct the struct type 
 */
      if( ierr == MPI_SUCCESS )
         ierr =   MPI_Type_struct( nbufs, blen, disp, type,
                                   &PANEL->dtypes[IBUF] );
/*
 * release temporaries
 */
      if( bufs ) free( bufs );
      if( blen ) free( blen );
      if( disp ) free( disp );
      if( type ) free( type );
/*
 * commit the type 
 */
      if( ierr == MPI_SUCCESS )
         ierr =   MPI_Type_commit( &PANEL->dtypes[IBUF] );

      return( ierr );
   }
   else
   {
/*
 * Memory allocation failed -> abort
 */
      HPL_pabort( __LINE__, "HPL_packL", "Memory allocation failed" );
      return( MPI_SUCCESS );    /* never executed (hopefully ...) */
   }
#endif
#else
          /* HPL_USE_MPI_DATATYPE not defined - Oops, there is a bug
             somewhere, so, just in case  and until I find it ... */
   return( MPI_SUCCESS );   
#endif
/*
 * End of HPL_packL
 */
}
