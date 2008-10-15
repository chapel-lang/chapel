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
void HPL_plindx1
(
   HPL_T_panel *                    PANEL,
   const int                        K,
   const int *                      IPID,
   int *                            IPA,
   int *                            LINDXA,
   int *                            LINDXAU,
   int *                            IPLEN,
   int *                            IPMAP,
   int *                            IPMAPM1,
   int *                            PERMU,
   int *                            IWORK
)
#else
void HPL_plindx1
( PANEL, K, IPID, IPA, LINDXA, LINDXAU, IPLEN, IPMAP, IPMAPM1, PERMU, IWORK )
   HPL_T_panel *                    PANEL;
   const int                        K;
   const int *                      IPID;
   int *                            IPA;
   int *                            LINDXA;
   int *                            LINDXAU;
   int *                            IPLEN;
   int *                            IPMAP;
   int *                            IPMAPM1;
   int *                            PERMU;
   int *                            IWORK;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_plindx1 computes two local arrays  LINDXA and  LINDXAU  containing
 * the  local  source and final destination position  resulting from the
 * application of row interchanges.  In addition, this function computes
 * three arrays IPLEN, IPMAP and IPMAPM1  that contain  the  logarithmic
 * mapping information for the spreading phase.
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
 * IPID    (global input)                const int *
 *         On entry,  IPID  is an array of length K. The first K entries
 *         of that array contain the src and final destination resulting
 *         from the application of the interchanges.
 *
 * IPA     (global output)               int *
 *         On exit,  IPA  specifies  the number of rows that the current
 *         process row has that either belong to U  or should be swapped
 *         with remote rows of A.
 *
 * LINDXA  (global output)               int *
 *         On entry, LINDXA  is an array of dimension 2*N. On exit, this
 *         array contains the local indexes of the rows of A I have that
 *         should be copied into U.
 *
 * LINDXAU (global output)               int *
 *         On exit, LINDXAU  is an array of dimension 2*N. On exit, this
 *         array contains  the local destination  information encoded as
 *         follows.  If LINDXAU(k) >= 0, row  LINDXA(k)  of A  is  to be
 *         copied in U at position LINDXAU(k).  Otherwise, row LINDXA(k)
 *         of A should be locally copied into A(-LINDXAU(k),:).
 *
 * IPLEN   (global output)               int *
 *         On entry, IPLEN is an array of dimension NPROW + 1. On  exit,
 *         this array is such that  IPLEN[i]  is the number of rows of A
 *         in  the  processes  before  process  IPMAP[i]  after the sort
 *         with the convention that IPLEN[nprow]  is the total number of
 *         rows of the panel.  In other words IPLEN[i+1]-IPLEN[i] is the
 *         local number of rows of A that should be moved to the process
 *         IPMAP[i]. IPLEN is such that the number of rows of the source
 *         process  row can be computed as  IPLEN[1] - IPLEN[0], and the
 *         remaining  entries  of  this  array  are  sorted  so that the
 *         quantities IPLEN[i+1] - IPLEN[i] are logarithmically sorted.
 *
 * IPMAP   (global output)               int *
 *         On entry, IPMAP is an array of dimension NPROW. On exit, this
 *         array contains  the logarithmic mapping of the processes.  In
 *         other words, IPMAP[myrow] is the corresponding sorted process
 *         coordinate.
 *
 * IPMAPM1 (global output)               int *
 *         On entry, IPMAPM1  is an array of dimension NPROW.  On  exit,
 *         this  array  contains  the inverse of the logarithmic mapping
 *         contained  in  IPMAP:  IPMAPM1[ IPMAP[i] ] = i,  for all i in
 *         [0.. NPROCS)
 *
 * PERMU   (global output)               int *
 *         On entry,  PERMU  is an array of dimension JB. On exit, PERMU
 *         contains  a sequence of permutations,  that should be applied
 *         in increasing order to permute in place the row panel U.
 *
 * IWORK   (workspace)                   int *
 *         On entry, IWORK is a workarray of dimension 2*JB.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   int                        * iwork;
   int                        dst, dstrow, fndd, i, ia, icurrow, il,
                              ip, ipU, iroff, j, jb, myrow, nb, nprow,
                              src, srcrow;
/* ..
 * .. Executable Statements ..
 */
/*
 * Logarithmic sort of the processes - compute IPMAP, IPLEN and IPMAPM1
 */
   HPL_plindx10( PANEL, K, IPID, IPLEN, IPMAP, IPMAPM1 );
/*
 * Compute the local arrays  LINDXA  and  LINDXAU  containing  the local
 * source and final destination position resulting from  the application
 * of N interchanges. Compute LINDXA and LINDXAU in icurrow,  and LINDXA
 * elsewhere and PERMU in every process.
 */
   myrow = PANEL->grid->myrow; nprow   = PANEL->grid->nprow;
   jb    = PANEL->jb;          nb      = PANEL->nb;     ia = PANEL->ia;
   iroff = PANEL->ii;          icurrow = PANEL->prow;

   iwork = IWORK + jb;
 
   if( myrow == icurrow )
   {
      for( i = 0, ip = 0, ipU = 0; i < K; i += 2 )
      {
         src = IPID[i]; Mindxg2p( src, nb, nb, srcrow, 0, nprow );
 
         if( srcrow == icurrow )
         {
            dst = IPID[i+1]; Mindxg2p( dst, nb, nb, dstrow, 0, nprow );
 
            Mindxg2l( il, src, nb, nb, myrow, 0, nprow );
            LINDXA[ip] = il - iroff;
 
            if( ( dstrow == icurrow ) && ( dst - ia < jb ) )
            {
               PERMU[ipU] = dst - ia;  il = IPMAPM1[dstrow];
               j          = IPLEN[il]; iwork[ipU] = LINDXAU[ip] = j;
               IPLEN[il]++; ipU++;
            }
            else if( dstrow != icurrow )
            {
               j = 0;
               do { fndd = ( dst == IPID[j] ); j+=2; }
               while( !fndd && ( j < K ) );
 
               PERMU[ipU] = IPID[j-1]-ia; il = IPMAPM1[dstrow];
               j          = IPLEN[il];    iwork[ipU] = LINDXAU[ip] = j;
               IPLEN[il]++; ipU++;
            }
            else if( ( dstrow == icurrow ) && ( dst - ia >= jb ) )
            {
               Mindxg2l( il, dst, nb, nb, myrow, 0, nprow );
               LINDXAU[ip] = iroff - il;
            }
            ip++;
         }
      }
      *IPA = ip;
   }
   else
   {
      for( i = 0, ip = 0, ipU = 0; i < K; i += 2 )
      {
         src = IPID[i  ]; Mindxg2p( src, nb, nb, srcrow, 0, nprow );
         dst = IPID[i+1]; Mindxg2p( dst, nb, nb, dstrow, 0, nprow );
/*
 * LINDXA[i] is the local index of the row of A that belongs into U
 */
         if( myrow == dstrow )
         {
            Mindxg2l( il, dst, nb, nb, myrow, 0, nprow );
            LINDXA[ip] = il - iroff; ip++;
         }
/*
 * iwork[i] is the local (current) position  index in U
 * PERMU[i] is the local (final) destination index in U
 */
         if( srcrow == icurrow )
         {
            if( ( dstrow == icurrow ) && ( dst - ia < jb ) )
            {
               PERMU[ipU] = dst - ia;  il = IPMAPM1[dstrow];
               iwork[ipU] = IPLEN[il]; IPLEN[il]++; ipU++;
            }
            else if( dstrow != icurrow )
            {
               j = 0;
               do { fndd = ( dst == IPID[j] ); j+=2; }
               while( !fndd && ( j < K ) );
               PERMU[ipU] = IPID[j-1] - ia; il = IPMAPM1[dstrow];
               iwork[ipU] = IPLEN[il]; IPLEN[il]++; ipU++;
            }
         }
      }
      *IPA = 0;
   }
/*
 * Simplify iwork and PERMU, return in PERMU the sequence of permutation
 * that need to be apply to U after it has been broadcast.
 */
   HPL_perm( jb, iwork, PERMU, IWORK );
/*
 * Reset IPLEN to its correct value
 */
   for( i = nprow; i > 0; i-- ) IPLEN[i] = IPLEN[i-1];
   IPLEN[0] = 0; 
/*
 * End of HPL_plindx1
 */
}
