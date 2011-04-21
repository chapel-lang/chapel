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

#ifndef _NANOS_ATOMIC_DECL
#define _NANOS_ATOMIC_DECL

#include "compatibility.hpp"
#include "nanos-int.h"
#include <algorithm> // for min/max

/* TODO: move to configure
#include <ext/atomicity.h>
#ifndef _GLIBCXX_ATOMIC_BUILTINS
#error "Atomic gcc builtins support is mandatory at this point"
#endif
*/


namespace nanos
{

   template<typename T>

   class Atomic
   {

      private:
         volatile T     _value;

      public:
         // constructor
         Atomic () {}

         Atomic ( T init ) : _value( init ) {}

         // copy constructor
         Atomic ( const Atomic &atm ) : _value( atm._value ) {}

         // assignment operator
         Atomic & operator= ( const Atomic &atm );
         Atomic & operator= ( const T val );
         // destructor
         ~Atomic() {}

         T fetchAndAdd ( const T& val=1 );
         T addAndFetch ( const T& val=1 );
         T fetchAndSub ( const T& val=1 );
         T subAndFetch ( const T& val=1 );
         T value() const;

         //! pre-increment ++
         T operator++ ();
         T operator-- ();

         //! post-increment ++
         T operator++ ( int val );
         T operator-- ( int val );

         //! += operator
         T operator+= ( const T val );
         T operator+= ( const Atomic<T> &val );

         T operator-= ( const T val );
         T operator-= ( const Atomic<T> &val );

         //! equal operator
         bool operator== ( const Atomic<T> &val );
         bool operator!= ( const Atomic<T> &val );

         bool operator< (const Atomic<T> &val );
         bool operator> ( const Atomic<T> &val );
         bool operator<= ( const Atomic<T> &val );
         bool operator>= ( const Atomic<T> &val );

         // other atomic operations

         //! compare and swap
         bool cswap ( const Atomic<T> &oldval, const Atomic<T> &newval );

         volatile T & override ();
   };

   void memoryFence ();
   template<typename T>
   bool compareAndSwap( T *ptr, T oldval, T  newval );

   class Lock : public nanos_lock_t
   {

      private:
         typedef nanos_lock_state_t state_t;

         // disable copy constructor and assignment operator
         Lock( const Lock &lock );
         const Lock & operator= ( const Lock& );

      public:
         // constructor
         Lock( state_t init=NANOS_LOCK_FREE ) : nanos_lock_t( init ) {};

         // destructor
         ~Lock() {}

         void acquire ( void );
         bool tryAcquire ( void );
         void release ( void );

         state_t operator* () const;

         state_t getState () const;

         void operator++ ( int val );

         void operator-- ( int val );
   };

   class LockBlock
   {
     private:
       Lock & _lock;

       // disable copy-constructor
       explicit LockBlock ( const LockBlock & );

     public:
       LockBlock ( Lock & lock );
       ~LockBlock ( );

       void acquire();
       void release();
   };

   class SyncLockBlock : public LockBlock
   {
     private:
       // disable copy-constructor
       explicit SyncLockBlock ( const SyncLockBlock & );

     public:
       SyncLockBlock ( Lock & lock );
       ~SyncLockBlock ( );
   };

};

#endif
