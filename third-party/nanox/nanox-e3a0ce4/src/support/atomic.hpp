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

#ifndef _NANOS_ATOMIC
#define _NANOS_ATOMIC

#include "atomic_decl.hpp"
#include "compatibility.hpp"
#include "nanos-int.h"
#include <algorithm> // for min/max

/* TODO: move to configure
#include <ext/atomicity.h>
#ifndef _GLIBCXX_ATOMIC_BUILTINS
#error "Atomic gcc builtins support is mandatory at this point"
#endif
*/


using namespace nanos;

template<typename T>
inline T Atomic<T>::fetchAndAdd ( const T& val )
{
   return __sync_fetch_and_add( &_value,val );
}

template<typename T>
inline T Atomic<T>::addAndFetch ( const T& val )
{
   return __sync_add_and_fetch( &_value,val );
}

template<typename T>
inline T Atomic<T>::fetchAndSub ( const T& val )
{
   return __sync_fetch_and_sub( &_value,val );
}

template<typename T>
inline T Atomic<T>::subAndFetch ( const T& val )
{
   return __sync_sub_and_fetch( &_value,val );
}

template<typename T>
inline T Atomic<T>::value() const
{
   return _value;
}

template<typename T>
inline T Atomic<T>::operator++ ()
{
   return addAndFetch();
}

template<typename T>
inline T Atomic<T>::operator-- ()
{
   return subAndFetch();
}

template<typename T>
inline T Atomic<T>::operator++ ( int val )
{
   return fetchAndAdd();
}

template<typename T>
inline T Atomic<T>::operator-- ( int val )
{
   return fetchAndSub();
}

template<typename T>
inline T Atomic<T>::operator+= ( const T val )
{
   return addAndFetch(val);
}

template<typename T>
inline T Atomic<T>::operator+= ( const Atomic<T> &val )
{
   return addAndFetch(val.value());
}

template<typename T>
inline T Atomic<T>::operator-= ( const T val )
{
   return subAndFetch(val);
}

template<typename T>
inline T Atomic<T>::operator-= ( const Atomic<T> &val )
{
   return subAndFetch(val.value());
}

template<typename T>
inline bool Atomic<T>::operator== ( const Atomic<T> &val )
{
   return value() == val.value();
}

template<typename T>
inline bool Atomic<T>::operator!= ( const Atomic<T> &val )
{
   return value() != val.value();
}

template<typename T>
inline bool Atomic<T>::operator< (const Atomic<T> &val )
{
   return value() < val.value();
}

template<typename T>
inline bool Atomic<T>::operator> ( const Atomic<T> &val )
{
   return value() > val.value();
}

template<typename T>
inline bool Atomic<T>::operator<= ( const Atomic<T> &val )
{
   return value() <= val.value();
}

template<typename T>
inline bool Atomic<T>::operator>= ( const Atomic<T> &val )
{
   return value() >= val.value();
}

template<typename T>
inline bool Atomic<T>::cswap ( const Atomic<T> &oldval, const Atomic<T> &newval )
{
   return __sync_bool_compare_and_swap ( &_value, oldval.value(), newval.value() );
}

template<typename T>
inline volatile T & Atomic<T>::override ()
{
    return _value;
}

template<typename T>
inline Atomic<T> & Atomic<T>::operator= ( const T val )
{
   this->_value = val;
   return *this;
}

template<typename T>
inline Atomic<T> & Atomic<T>::operator= ( const Atomic<T> &val )
{
   return operator=( val._value );
}

inline Lock::state_t Lock::operator* () const
{
   return _state;
}

inline Lock::state_t Lock::getState () const
{
   return _state;
}

inline void Lock::operator++ ( int val )
{
   acquire();
}

inline void Lock::operator-- ( int val )
{
   release();
}

inline void Lock::acquire ( void )
{

spin:

   while ( _state == NANOS_LOCK_BUSY );

   if ( __sync_lock_test_and_set( &_state,NANOS_LOCK_BUSY ) ) goto spin;
}

inline bool Lock::tryAcquire ( void )
{
   if ( _state == NANOS_LOCK_FREE ) {
      if ( __sync_lock_test_and_set( &_state,NANOS_LOCK_BUSY ) ) return false;
      else return true;
   } else return false;
}

inline void Lock::release ( void )
{
   __sync_lock_release( &_state );
}

inline void nanos::memoryFence ()
{
    __sync_synchronize();
}

template<typename T>
inline bool nanos::compareAndSwap( T *ptr, T oldval, T  newval )
{
    return __sync_bool_compare_and_swap ( ptr, oldval, newval );
}

inline LockBlock::LockBlock ( Lock & lock ) : _lock(lock)
{
   acquire();
}

inline LockBlock::~LockBlock ( )
{
   release();
}

inline void LockBlock::acquire()
{
   _lock++;
}

inline void LockBlock::release()
{
   _lock--;
}

inline SyncLockBlock::SyncLockBlock ( Lock & lock ) : LockBlock(lock)
{
   memoryFence();
}

inline SyncLockBlock::~SyncLockBlock ( )
{
   memoryFence();
}

#endif
