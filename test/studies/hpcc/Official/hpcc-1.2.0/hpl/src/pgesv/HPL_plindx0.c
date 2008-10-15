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
void HPL_plindx0
(
   HPL_T_panel *                    PANEL,
   const int                        K,
   int *                            IPID,
   int *                            LINDXA,
   int *                            LINDXAU,
   int *                            LLEN
)
#else
void HPL_plindx0
( PANEL, K, IPID, LINDXA, LINDXAU, LLEN )
   HPL_T_panel *                    PANEL;
   const int                        K;
   int *                            IPID;
   int *                            LINDXA;
   int *                            LINDXAU;
   int *                            LLEN;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_plindx0 computes two local arrays  LINDXA and  LINDXAU  containing
 * the  local  source and final destination position  resulting from the
 * application of row interchanges.
 *  
 * On entry, the array  IPID  of length K is such that the row of global
 * index  IPID(i)  should be mapped onto row of global index  IPID(i+1).
 * Let  IA  be the global index of the first row to be swapped. For k in
 * [0..K/2), the row of global index IPID(2*k) should be mapped onto the
 * row of global index  IPID(2*k+1).  The question then, is to determine
 * which rows should ultimately be part of U.
 *  
 * First, some rows of the process ICURROW  may be swapped locally.  One
 * of this row belongs to U, the other one belongs to my local  piece of
 * A.  The other  rows of the current block are swapped with remote rows
 * and are thus not part of U. These rows however should be sent  along,
 * and  grabbed by the other processes  as we  progress in the  exchange
 * phase.
 *  
 * So, assume that I am  ICURROW  and consider a row of index  IPID(2*i)
 * that I own. If I own IPID(2*i+1) as well and IPID(2*i+1) - IA is less
 * than N,  this row is locally swapped and should be copied into  U  at
 * the position IPID(2*i+1) - IA. No row will be exchanged for this one.
 * If IPID(2*i+1)-IA is greater than N, then the row IPID(2*i) should be
 * locally copied into my local piece of A at the position corresponding
 * to the row of global index IPID(2*i+1).
 *  
 * If the process  ICURROW does not own  IPID(2*i+1), then row IPID(2*i)
 * is to be swapped away and strictly speaking does not belong to U, but
 * to  A  remotely.  Since this  process will however send this array U,
 * this row is  copied into  U, exactly where the row IPID(2*i+1) should
 * go. For this, we search IPID for k1, such that IPID(2*k1) is equal to
 * IPID(2*i+1); and row  IPID(2*i) is to be copied in U  at the position
 * IPID(2*k1+1)-IA.
 *  
 * It is thus  important to put the rows that go into U, i.e., such that
 * IPID(2*i+1) - IA is less than N at the begining of the array IPID. By
 * doing so,  U  is formed, and the local copy  is performed in just one
 * sweep.
 *  
 * Two lists  LINDXA  and  LINDXAU are built.  LINDXA contains the local
 * index of the rows I have that should be copied. LINDXAU  contains the
 * local destination information: if LINDXAU(k) >= 0, row LINDXA(k) of A
 * is to be copied in U at position LINDXAU(k). Otherwise, row LINDXA(k)
 * of A should be locally copied into A(-LINDXAU(k),:).  In the  process
 * ICURROW, the initial packing algorithm proceeds as follows.
 *  
 *   for all entries in IPID,
 *      if IPID(2*i) is in ICURROW,
 *         if IPID(2*i+1) is in ICURROW,
 *            if( IPID(2*i+1) - IA < N )
 *             save corresponding local position
 *             of this row (LINDXA);
 *             save local position (LINDXAU) in U
 *             where this row goes;
 *             [copy row IPID(2*i) in U at position
 *             IPID(2*i+1)-IA; ];
 *            else
 *             save corresponding local position of
 *             this row (LINDXA);
 *             save local position (-LINDXAU) in A
 *             where this row goes;
 *             [copy row IPID(2*i) in my piece of A
 *             at IPID(2*i+1);]
 *            end if
 *         else
 *            find k1 such that IPID(2*k1) = IPID(2*i+1);
 *            copy row IPID(2*i) in U at position
 *            IPID(2*k1+1)-IA;
 *            save corresponding local position of this
 *            row (LINDXA);
 *            save local position (LINDXAU) in U where
 *            this row goes;
 *         end if
 *      end if
 *   end for
 *  
 * Second, if I am not the current row process  ICURROW, all source rows
 * in IPID that I own are part of U. Indeed,  they  are swapped with one
 * row  of  the  current  block  of rows,  and  the  main  factorization
 * algorithm proceeds one row after each other.  The processes different
 * from ICURROW,  should  exchange and accumulate  those rows until they
 * receive some data previously owned by the process ICURROW.
 *  
 * In processes different from  ICURROW,  the  initial packing algorithm
 * proceeds as follows.  Consider a row of global index IPID(2*i) that I
 * own. When I will be receiving data previously owned by ICURROW, i.e.,
 * U, row IPID(2*i) should  replace the row in U at pos. IPID(2*i+1)-IA,
 * and  this particular row of U should be first copied into my piece of
 * A, at A(il,:),  where  il is the  local row  index  corresponding  to
 * IPID(2*i). Now,initially, this row will be packed into workspace, say
 * as the kth row of  that  work array.  The  following  algorithm  sets
 * LINDXAU[k] to IPID(2*i+1)-IA, that is the position in U where the row
 * should be copied. LINDXA(k) stores the local index in  A  where  this
 * row of U should be copied, i.e il.
 *  
 *   for all entries in IPID,
 *      if IPID(2*i) is not in ICURROW,
 *         copy row IPID(2*i) in work array;
 *         save corresponding local position
 *         of this row (LINDXA);
 *         save position (LINDXAU) in U where
 *         this row should be copied;
 *      end if
 *   end for
 *  
 * Since we are at it, we also globally figure  out  how many rows every
 * process has. That is necessary, because it would rather be cumbersome
 * to  figure it on  the fly  during the  bi-directional exchange phase.
 * This information is kept in the array  LLEN  of size NPROW. Also note
 * that the arrays LINDXA and LINDXAU are of max length equal to 2*N.
 *
 * Arguments
 * =========
 *
 * PANEL   (local input/output)          HPL_T_panel *
 *         On entry,  PANEL  points to the data structure containing the
 *         panel information.
 *
 * K       (global input)                const int
 *         On entry, K specifies the number of entries in IPID.  K is at
 *         least 2*N, and at most 4*N.
 *
 * IPID    (global input)                int *
 *         On entry,  IPID  is an array of length K. The first K entries
 *         of that array contain the src and final destination resulting
 *         from the application of the interchanges.
 *
 * LINDXA  (local output)                int *
 *         On entry, LINDXA  is an array of dimension 2*N. On exit, this
 *         array contains the local indexes of the rows of A I have that
 *         should be copied into U.
 *
 * LINDXAU (local output)                int *
 *         On exit, LINDXAU  is an array of dimension 2*N. On exit, this
 *         array contains  the local destination  information encoded as
 *         follows.  If LINDXAU(k) >= 0, row  LINDXA(k)  of A  is  to be
 *         copied in U at position LINDXAU(k).  Otherwise, row LINDXA(k)
 *         of A should be locally copied into A(-LINDXAU(k),:).
 *
 * LLEN    (global output)               int *
 *         On entry,  LLEN  is  an array  of length  NPROW.  On exit, it
 *         contains how many rows every process has.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   int                        dst, dstrow, fndd, i, ia, icurrow, il,
                              ip=0, iroff, j, jb, myrow, nb, nprow,
                              src, srcrow;
/* ..
 * .. Executable Statements ..
 */
/*
 * Compute the local arrays  LINDXA  and  LINDXAU  containing  the local
 * source and final destination position resulting from  the application
 * of N interchanges.
 */
   myrow   = PANEL->grid->myrow; nprow = PANEL->grid->nprow;
   icurrow = PANEL->prow;        jb    = PANEL->jb;
   nb      = PANEL->nb;          ia    = PANEL->ia;
   iroff   = PANEL->ii;

   for( i = 0; i < nprow; i++ ) LLEN[i] = 0;

   for( i = 0; i < K; i += 2 )
   {
      src = IPID[i];
      Mindxg2p( src, nb, nb, srcrow, 0, nprow ); LLEN[ srcrow ]++;

      if( myrow == srcrow )
      {
         Mindxg2l( il, src, nb, nb, myrow, 0, nprow );
         LINDXA[ip] = il - iroff; dst = IPID[i+1];

         if( myrow == icurrow )
         {
            Mindxg2p( dst, nb, nb, dstrow, 0, nprow );
            if( dstrow == icurrow )
            {
               if( dst - ia < jb ) { LINDXAU[ip] = dst - ia; }
               else
               {
                  Mindxg2l( il, dst, nb, nb, myrow, 0, nprow );
                  LINDXAU[ip] = iroff - il;
               }
            }
            else
            {
               j = 0;
               do { fndd = ( dst == IPID[j] ); j+=2; }
               while( !fndd && ( j < K ) );
               LINDXAU[ip] = IPID[j-1] - ia;
            }
         }
         else { LINDXAU[ip] = dst - ia; }

         ip++;
      }
   }
/*
 * End of HPL_plindx0
 */
}
