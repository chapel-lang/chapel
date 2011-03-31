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

#ifndef _NANOS_LIB_WDDEQUE
#define _NANOS_LIB_WDDEQUE

#include "wddeque_decl.hpp"
#include "schedule.hpp"
#include "system.hpp"
#include "instrumentation.hpp"
#include "atomic.hpp"

using namespace nanos;

inline bool WDDeque::empty ( void ) const
{
   return _dq.empty();
}

inline size_t WDDeque::size() const
{
   return _dq.size();
}


inline void WDDeque::push_front ( WorkDescriptor *wd )
{
   wd->setMyQueue( this );
   {
      LockBlock lock( _lock );
      _dq.push_front( wd );
      int tasks = ++( sys.getSchedulerStats()._readyTasks );
      increaseTasksInQueues(tasks);
      memoryFence();
   }
}

inline void WDDeque::push_back ( WorkDescriptor *wd )
{
   wd->setMyQueue( this );
   {
      LockBlock lock( _lock );
      _dq.push_back( wd );
      int tasks = ++( sys.getSchedulerStats()._readyTasks );
      increaseTasksInQueues(tasks);
      memoryFence();
   }
}

struct NoConstraints
{
   static inline bool check ( WD &wd, BaseThread &thread ) { return true; }
};

inline WorkDescriptor * WDDeque::pop_front ( BaseThread *thread )
{
  return popFrontWithConstraints<NoConstraints>(thread);
}

inline WorkDescriptor * WDDeque::pop_back ( BaseThread *thread )
{
  return popFrontWithConstraints<NoConstraints>(thread);
}

inline bool WDDeque::removeWD( BaseThread *thread, WorkDescriptor *toRem, WorkDescriptor **next )
{
  return removeWDWithConstraints<NoConstraints>(thread,toRem,next);
}

template <typename Constraints>
inline WorkDescriptor * WDDeque::popFrontWithConstraints ( BaseThread *thread )
{
   WorkDescriptor *found = NULL;

   if ( _dq.empty() )
      return NULL;

   {
      LockBlock lock( _lock );

      memoryFence();

      if ( !_dq.empty() ) {
         WDDeque::BaseContainer::iterator it;

         for ( it = _dq.begin() ; it != _dq.end(); it++ ) {
            WD &wd = *(WD *)*it; 
            if ( Scheduler::checkBasicConstraints( wd, *thread) && Constraints::check(wd,*thread) ) {
               if ( wd.dequeue( &found ) ) {
                   _dq.erase( it );
                   int tasks = --(sys.getSchedulerStats()._readyTasks);
                   decreaseTasksInQueues(tasks);
               }
               break;
            }
         }
      }

      if ( found != NULL ) found->setMyQueue( NULL );

   }

   ensure( !found || !found->isTied() || found->isTiedTo() == thread, "" );

   return found;
}


// Only ensures tie semantics
template <typename Constraints>
inline WorkDescriptor * WDDeque::popBackWithConstraints ( BaseThread *thread )
{
   WorkDescriptor *found = NULL;

   if ( _dq.empty() )
      return NULL;

   {
      LockBlock lock( _lock );

      memoryFence();
   
      if ( !_dq.empty() ) {
         WDDeque::BaseContainer::reverse_iterator rit;
   
         for ( rit = _dq.rbegin(); rit != _dq.rend() ; rit++ ) {
            WD &wd = *(WD *)*rit; 
            if ( Scheduler::checkBasicConstraints( wd, *thread) && Constraints::check(wd,*thread)) {
               if ( wd.dequeue( &found ) ) {
                  _dq.erase( ( ++rit ).base() );
                  int tasks = --(sys.getSchedulerStats()._readyTasks);
                  decreaseTasksInQueues(tasks);
               }
               break;
            }
         }
      }
   
      if ( found != NULL ) found->setMyQueue( NULL );
   
   }

   ensure( !found || !found->isTied() || found->isTiedTo() == thread, "" );

   return found;
}


template <typename Constraints>
inline bool WDDeque::removeWDWithConstraints( BaseThread *thread, WorkDescriptor *toRem, WorkDescriptor **next )
{
   if ( _dq.empty() ) return false;

   if ( !Scheduler::checkBasicConstraints( *toRem, *thread) || !Constraints::check(*toRem, *thread) ) return false;

   *next = NULL;
   WDDeque::BaseContainer::iterator it;

   {
      LockBlock lock( _lock );

      memoryFence();

      if ( !_dq.empty() && toRem->getMyQueue() == this ) {
         for ( it = _dq.begin(); it != _dq.end(); it++ ) {
            if ( *it == toRem ) {
               if ( ( *it )->dequeue( next ) ) {
                  _dq.erase( it );
                  int tasks = --(sys.getSchedulerStats()._readyTasks);
                  decreaseTasksInQueues(tasks);
               }
               (*next)->setMyQueue( NULL );
               return true;
            }
         }
      }
   }

   return false;
}

inline void WDDeque::increaseTasksInQueues( int tasks )
{
   NANOS_INSTRUMENT(static nanos_event_key_t key = sys.getInstrumentation()->getInstrumentationDictionary()->getEventKey("num-ready");)
   NANOS_INSTRUMENT(sys.getInstrumentation()->raisePointEvent( key, (nanos_event_value_t) tasks );)
}

inline void WDDeque::decreaseTasksInQueues( int tasks )
{
   NANOS_INSTRUMENT(static nanos_event_key_t key = sys.getInstrumentation()->getInstrumentationDictionary()->getEventKey("num-ready");)
   NANOS_INSTRUMENT(sys.getInstrumentation()->raisePointEvent( key, (nanos_event_value_t) tasks );)
}

#endif

