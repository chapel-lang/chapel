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
 * Initial STACK state for PPC 32
 * -------------------------------------------------------------------
 *
 *  +------------------------------+
 *  |                              |
 *  +------------------------------+ <- state
 *  |    scratch area for helper   |
 *  +------------------------------+
 *  |     (20)  r12                |
 *  |     (24)  r14 =  userf       |
 *  |     (28)  r15 =  data        |
 *  |     (32)  r16 =  cleanup     |
 *  |     (36)  r17 =  clenaup arg |
 *  |           ....               |
 *  |     (92)  r31                |
 *  |     (96)  f14   (8 bytes)    |
 *  |           ....               |
 *  |    (232)  f31   (8 bytes)    |
 *  |    (240)  back chain = 0     |
 *  |    (244)  r0  =  startHelper |
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

   state -= 62; // 244/sizeof(intptr_t)

   // return link
   state[61] = (intptr_t) startHelper;
   // back chain
   state[60] = 0;
      
   // (r14) userf
   state[6] = (intptr_t) userFunction;
   // (r15) data
   state[7] = (intptr_t) userArg;
   // (r16) cleanup
   state[8] = (intptr_t) cleanup;
   // (r17) cleanup arg
   state[9] = (intptr_t) cleanupArg;

   return state;
}
