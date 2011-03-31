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


/* -------------------------------------------------------------------
 * Initial STACK _state for IA-64
 * -------------------------------------------------------------------
 *
 *  +------------------------------+
 *  | (00) f31                     |<-[context]
 *  | (02) f30                     |
 *  | (..)                         |
 *  | (30) f16                     |
 *  | (32) f5                      |
 *  | (..)                         |
 *  | (38) f2                      |
 *  | (40) ar.fpsr                 |
 *  | (41) b5                      |
 *  | (..)                         |
 *  | (45) b1                      |
 *  | (46) ar.lc                   |
 *  | (47) pr                      |
 *  | (48) ar.unat(callee)         |
 *  | (49) r7 == 'user func'       |
 *  | (50) r6 == 'cleanup arg'     |
 *  | (51) r5 == 'cleanup'         |
 *  | (52) r4                      |
 *  | (53) ar.unat(caller)         |
 *  | (54) tp (r13)                |
 *  | (55) ar.bspstore             |
 *  | (56) ar.rnat                 |
 *  | (57) ar.pfs                  |
 *  | (58) ar.rsc                  |
 *  | (59) rp == 'startHelper'     |
 *  +------------------------------+
 *  | (60) arg                     |
 *  +------------------------------+
 *
 * -----------------------------------------------------------------*/

extern "C"
{
// low-level helper routine to start a new user-thread
   void startHelper ();
}

intptr_t * initContext ( intptr_t *stack, size_t stackSize, void *userFunction, void *userArg,
                          void *cleanup, void *cleanupArg )
{
   // stack grows down
   intptr_t *state = stack;
   state += stackSize;

   state -= 68;

   // argument
   state[60] = ( intptr_t ) userArg;

   // return pointer
   // The union here avoids breaking strict C aliasing rules for type-punning
   // A simple cast wouldn't have worked 
   union {
     long * address;
     void (*ptrfun) (); 
   } r;

   r.ptrfun = startHelper;
   state[59] = ( intptr_t ) *r.address;
   // ar.rsc
   state[58] = ( intptr_t ) 3;
   // ar.pfs
   state[57] = ( intptr_t ) 0;
   // ar.rnat
   state[56] = ( intptr_t ) 0;
   // ar.bspstore
   state[55] = ( intptr_t ) stack;
   // tp(r13)
   state[54] = ( intptr_t ) 0;
   // ar.unat (caller)
   state[53] = ( intptr_t ) 0;
   // r5 (cleanup)
   state[51] = ( intptr_t ) cleanup;
   // r6 (pt)
   state[50] = ( intptr_t ) cleanupArg;
   // r7 (userf)
   state[49] = ( intptr_t ) userFunction;
   // ar.unat (callee)
   state[48] = ( intptr_t ) 0;
   // ar.fpsr
   state[40] = ( intptr_t ) 0x9804c0270033f;

   return state;
}
