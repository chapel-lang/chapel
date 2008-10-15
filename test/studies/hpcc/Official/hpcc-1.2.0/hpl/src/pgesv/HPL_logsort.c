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
void HPL_logsort
(
   const int                        NPROCS,
   const int                        ICURROC,
   int *                            IPLEN,
   int *                            IPMAP,
   int *                            IPMAPM1
)
#else
void HPL_logsort
( NPROCS, ICURROC, IPLEN, IPMAP, IPMAPM1 )
   const int                        NPROCS;
   const int                        ICURROC;
   int *                            IPLEN;
   int *                            IPMAP;
   int *                            IPMAPM1;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_logsort computes an array  IPMAP  and  its inverse  IPMAPM1  that
 * contain  the logarithmic sorted processes id with repect to the local
 * number of rows of  U  that they own. This is necessary to ensure that
 * the logarithmic spreading of U is optimal in terms of number of steps
 * and communication volume as well.  In other words,  the larget pieces
 * of U will be sent a minimal number of times.
 *
 * Arguments
 * =========
 *
 * NPROCS  (global input)                const int
 *         On entry, NPROCS  specifies the number of process rows in the
 *         process grid. NPROCS is at least one.
 *
 * ICURROC (global input)                const int
 *         On entry, ICURROC is the source process row.
 *
 * IPLEN   (global input/output)         int *
 *         On entry, IPLEN is an array of dimension NPROCS+1,  such that
 *         IPLEN[0] is 0, and IPLEN[i] contains the number of rows of U,
 *         that process i-1 has.  On exit,  IPLEN[i]  is  the number  of
 *         rows of U  in the processes before process IPMAP[i] after the
 *         sort,  with  the convention that  IPLEN[NPROCS] is  the total
 *         number  of rows  of the panel.  In other words,  IPLEN[i+1] -
 *         IPLEN[i] is  the  number of rows of A that should be moved to
 *         the process IPMAP[i].  IPLEN  is such that the number of rows
 *         of  the  source process  row is IPLEN[1] - IPLEN[0],  and the
 *         remaining  entries  of  this  array  are  sorted  so that the
 *         quantities IPLEN[i+1]-IPLEN[i] are logarithmically sorted.
 *
 * IPMAP   (global output)               int *
 *         On entry,  IPMAP  is an array of dimension  NPROCS.  On exit,
 *         array contains  the logarithmic mapping of the processes.  In
 *         other words, IPMAP[myroc] is the corresponding sorted process
 *         coordinate.
 *
 * IPMAPM1 (global output)               int *
 *         On entry, IPMAPM1  is an array of dimension NPROCS.  On exit,
 *         this  array  contains  the inverse of the logarithmic mapping
 *         contained  in  IPMAP:  IPMAPM1[ IPMAP[i] ] = i,  for all i in
 *         [0.. NPROCS)
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   int                        dist, i, ip, iplen_i, iplen_j, itmp, j, k;
/* ..
 * .. Executable Statements ..
 */
/*
 * Compute the  logarithmic distance between process j and process 0, as
 * well as the maximum logarithmic distance. IPMAPM1 is workarray here.
 */
   for( j = 0, dist = 0; j < NPROCS; j++ )
   {
      IPMAP[j] = MModAdd( j, ICURROC, NPROCS ); ip = j; itmp = 0;
      do { if( ip & 1 ) itmp++; ip >>= 1; } while ( ip );
      IPMAPM1[j] = itmp; if( itmp > dist ) dist = itmp;
   }
/*
 * Shift IPLEN[1..NPROCS]  of ICURROC places,  so that  IPLEN[1]  is now
 * what used to be IPLEN[ICURROC+1]. Initialize IPMAP,  so that IPMAP[0]
 * is ICURROC.
 */
   for( j = 0; j < ICURROC; j++ )
   {
      for( i = 2, itmp = IPLEN[1]; i <= NPROCS; i++ ) IPLEN[i-1] = IPLEN[i];
      IPLEN[NPROCS] = itmp;
   }
/*
 * logarithmic sort
 */
   for( k = 1; k <= dist; k++ )
   {
      for( j = 1; j < NPROCS; j++ )
      {
         if( IPMAPM1[j] == k )
         {
            for( i = 2; i < NPROCS; i++ )
            {
               if( k < IPMAPM1[i] )
               {
                  iplen_i = IPLEN[i+1]; iplen_j = IPLEN[j+1];

                  if( iplen_j < iplen_i )
                  {
                     IPLEN[j+1] = iplen_i;  IPLEN[i+1] = iplen_j;
                     itmp       = IPMAP[j]; IPMAP[j]   = IPMAP[i];
                     IPMAP[i]   = itmp;
                  }
               }
            }
         }
      }
   }
/*
 * Compute IPLEN and IPMAPM1 (the inverse of IPMAP)
 */
   IPLEN[0] = 0;

   for( i = 0; i < NPROCS; i++ )
   {
      IPMAPM1[ IPMAP[i] ] = i;
      IPLEN[i+1]         += IPLEN[i];
   }
/*
 * End of HPL_logsort
 */
}
