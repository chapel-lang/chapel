/*************************************************************************************/
/*      Copyright 2009 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/

#include "smp_ult.hpp"

extern "C"
{
// low-level helper routine to start a new user-thread
    void startHelper ();
}

/*! \brief initializes a stack state for PowerPC32
 *
 * -------------------------------------------------------------------
 * Initial STACK state for PPC 64
 * -------------------------------------------------------------------
 *
 *  +------------------------------+
 *  |                              |
 *  +------------------------------+ <- state
 *  |    scratch area for helper   |
 *  +------------------------------+
 *  |      (8)  r12                |
 *  |     (16)  next LR            |
 *  |     (40)  TOC(r2)            |
 *  |     (48)  arg0               |
 *  |     (56)  arg1               |
 *  |     (64)  arg2               |
 *  |     (72)  r14 =  userf       |
 *  |     (80)  r15 =  data        |
 *  |     (88)  r16 =  cleanup     |
 *  |     (96)  r17 =  clenaup arg |
 *  |           ....               |
 *  |    (208)  r31                |
 *  |    (216)  f14   (8 bytes)    |
 *  |           ....               |
 *  |    (352)  f31   (8 bytes)    |
 *  |    (360)  back chain = 0     |
 *  |    (368)  r0  =  startHelper |
 *  +------------------------------+
 *
 * \sa switchStacks
 */

intptr_t * initContext ( intptr_t *stack, size_t stackSize, void *userFunction, void *userArg,
                          void *cleanup, void *cleanupArg )
{
   // stack grows down
   intptr_t *state = stack;
   state += stackSize;
   intptr_t *tmpStartHelperPtr = ( intptr_t * ) startHelper;

   state -= 53; // (368 + 56)/sizeof(intptr_t)

   // return link
   state[48] = *tmpStartHelperPtr;
   // back chain
   state[44] = 0;
      
   // (r14) userf
   state[9] = ( (intptr_t ) userFunction);
   // (r15) data
   state[10] = (intptr_t) userArg;
   // (r16) cleanup
   state[11] = (intptr_t) cleanup;
   // (r17) cleanup arg
   state[12] = (intptr_t) cleanupArg;

   return state;
}

