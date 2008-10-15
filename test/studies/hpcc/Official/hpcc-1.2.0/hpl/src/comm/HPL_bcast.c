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
int HPL_bcast
(
   HPL_T_panel *                    PANEL,
   int *                            IFLAG
)
#else
int HPL_bcast
( PANEL, IFLAG )
   HPL_T_panel *                    PANEL;
   int *                            IFLAG;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_bcast broadcasts  the  current  panel.  Successful  completion is
 * indicated by IFLAG set to HPL_SUCCESS on return. IFLAG will be set to
 * HPL_FAILURE on failure and to HPL_KEEP_TESTING when the operation was
 * not completed, in which case this function should be called again.
 *
 * Arguments
 * =========
 *
 * PANEL   (input/output)                HPL_T_panel *
 *         On entry,  PANEL  points to the  current panel data structure
 *         being broadcast.
 *
 * IFLAG   (output)                      int *
 *         On exit,  IFLAG  indicates  whether  or not the broadcast has
 *         occured.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   int                        ierr;
   HPL_T_TOP                  top;
/* ..
 * .. Executable Statements ..
 */
   if( PANEL == NULL ) { *IFLAG = HPL_SUCCESS; return( HPL_SUCCESS ); }
   if( PANEL->grid->npcol <= 1 )
   {                     *IFLAG = HPL_SUCCESS; return( HPL_SUCCESS ); }
/*
 * Retrieve the selected virtual broadcast topology
 */
   top = PANEL->algo->btopo;

   switch( top )
   {
      case HPL_1RING_M : ierr = HPL_bcast_1rinM( PANEL, IFLAG ); break;
      case HPL_1RING   : ierr = HPL_bcast_1ring( PANEL, IFLAG ); break;
      case HPL_2RING_M : ierr = HPL_bcast_2rinM( PANEL, IFLAG ); break;
      case HPL_2RING   : ierr = HPL_bcast_2ring( PANEL, IFLAG ); break;
      case HPL_BLONG_M : ierr = HPL_bcast_blonM( PANEL, IFLAG ); break;
      case HPL_BLONG   : ierr = HPL_bcast_blong( PANEL, IFLAG ); break;
      default          : ierr = HPL_SUCCESS;
   }
 
   return( ierr );
/*
 * End of HPL_bcast
 */
}
