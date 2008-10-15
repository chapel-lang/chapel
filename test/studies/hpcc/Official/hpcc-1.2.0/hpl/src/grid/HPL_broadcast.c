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
int HPL_broadcast
(
   void *                           BUFFER,
   const int                        COUNT,
   const HPL_T_TYPE                 DTYPE,
   const int                        ROOT,
   MPI_Comm                         COMM
)
#else
int HPL_broadcast
( BUFFER, COUNT, DTYPE, ROOT, COMM )
   void *                           BUFFER;
   const int                        COUNT;
   const HPL_T_TYPE                 DTYPE;
   const int                        ROOT;
   MPI_Comm                         COMM;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_broadcast broadcasts  a message from the process with rank ROOT to
 * all processes in the group.
 *
 * Arguments
 * =========
 *
 * BUFFER  (local input/output)          void *
 *         On entry,  BUFFER  points to  the  buffer to be broadcast. On
 *         exit, this array contains the broadcast data and is identical
 *         on all processes in the group.
 *
 * COUNT   (global input)                const int
 *         On entry,  COUNT  indicates the number of entries in  BUFFER.
 *         COUNT must be at least zero.
 *
 * DTYPE   (global input)                const HPL_T_TYPE
 *         On entry,  DTYPE  specifies the type of the buffers operands.
 *
 * ROOT    (global input)                const int
 *         On entry, ROOT is the coordinate of the source process.
 *
 * COMM    (global/local input)          MPI_Comm
 *         The MPI communicator identifying the process collection.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   int                        hplerr=MPI_SUCCESS, ip2=1, kk, mask=1, 
                              mpierr, mydist, partner, rank, size, 
                              tag = MSGID_BEGIN_COLL;
   MPI_Status                 status;
/* ..
 * .. Executable Statements ..
 */
   if( COUNT <= 0 ) return( MPI_SUCCESS );
   mpierr = MPI_Comm_size( COMM, &size ); if( size <= 1 ) return( mpierr );
   mpierr = MPI_Comm_rank( COMM, &rank );

   kk = size - 1;
   while( kk > 1 ) { kk >>= 1; ip2 <<= 1; mask <<= 1; mask++; }
   mydist = MModSub( rank, ROOT, size );

   do
   {
      mask ^= ip2;
      if( ( mydist & mask ) == 0 )
      {
         partner = mydist ^ ip2;

         if( mydist & ip2 )
         {
            partner = MModAdd( ROOT, partner, size );
            mpierr  = MPI_Recv(  BUFFER, COUNT, HPL_2_MPI_TYPE( DTYPE ),
                                 partner, tag, COMM, &status );
         }
         else if( partner < size )
         {
            partner = MModAdd( ROOT, partner, size );
            mpierr  = MPI_Send( BUFFER, COUNT, HPL_2_MPI_TYPE( DTYPE ),
                                partner, tag, COMM );
         }
         if( mpierr != MPI_SUCCESS ) hplerr = mpierr;
      }
      ip2 >>= 1;
   } while( ip2 );

   return( hplerr );
/*
 * End of HPL_broadcast
 */
}
