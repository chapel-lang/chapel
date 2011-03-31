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

#ifndef _NANOS_LIB_QUEUE_DECL
#define _NANOS_LIB_QUEUE_DECL

#include <queue>
#include "atomic_decl.hpp"
#include "debug.hpp"

namespace nanos
{

// FIX: implement own queue without coherence problems? lock-free?

   template<typename T> class Queue
   {

      private:
         typedef std::queue<T>   BaseContainer;
         Lock                    _qLock;
         BaseContainer           _q;

         // disable copy constructor and assignment operator
         Queue( Queue &orig );
         const Queue & operator= ( const Queue &orig );

      public:
         // constructors
         Queue() {}

         // destructor
         ~Queue() {}

         void push( T data );
         T    pop ( void );
         bool try_pop ( T& result );
   };

};

#endif

