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

#ifndef _NANOS_SYNCRHONIZED_CONDITION
#define _NANOS_SYNCRHONIZED_CONDITION

#include "synchronizedcondition_decl.hpp"
#include "atomic.hpp"
#include "basethread.hpp"
#include "schedule.hpp"

using namespace nanos;

inline void GenericSyncCond::lock()
{
   _lock.acquire();
   memoryFence();
}

inline void GenericSyncCond::unlock()
{
   memoryFence();
   _lock.release();
}

template <class _T>
void SynchronizedCondition< _T>::wait()
{
   Scheduler::waitOnCondition(this);
}

template <class _T>
void SynchronizedCondition< _T>::reference()
{
   _refcount++;
   memoryFence();
}

template <class _T>
void SynchronizedCondition< _T>::unreference()
{
   _refcount--;
}

template <class _T>
void SynchronizedCondition< _T>::waitConditionAndSignalers()
{
   Scheduler::waitOnCondition(this);
   while ( _refcount.value() > 0 ) {
      memoryFence();
   }
}

template <class _T>
void SynchronizedCondition< _T>::signal()
{
   lock();
   while ( hasWaiters() ) {
      WD* wd = getAndRemoveWaiter();
      Scheduler::wakeUp(wd);
   }
   unlock(); 
}

template <class _T>
void SynchronizedCondition< _T>::signal_one()
{
   lock();
   if ( hasWaiters() ) {
      WD* wd = getAndRemoveWaiter();
      Scheduler::wakeUp(wd);
   }
   unlock();
}

#endif

