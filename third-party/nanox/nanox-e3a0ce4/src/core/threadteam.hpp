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

#ifndef _NANOS_THREAD_TEAM_H
#define _NANOS_THREAD_TEAM_H
#include "threadteam_decl.hpp"
#include "atomic.hpp"
#include "debug.hpp"

using namespace nanos;

inline ThreadTeam::ThreadTeam ( int maxThreads, SchedulePolicy &policy, ScheduleTeamData *data, Barrier &barrier, ThreadTeamData & ttd, ThreadTeam * parent )
   : _idleThreads( 0 ), _numTasks( 0 ), _barrier(barrier), _singleGuardCount( 0 ),
     _schedulePolicy( policy ), _scheduleData( data ), _threadTeamData( ttd ), _parent( parent ), _level( parent == NULL ? 0 : parent->getLevel() + 1 ), _creatorId(-1)
{
      _threads.reserve( maxThreads );
}

inline ThreadTeam::~ThreadTeam ()
{
   delete &_barrier;
   ensure(size() == 0, "Destroying non-empty team!");
}

inline unsigned ThreadTeam::size() const
{
   return _threads.size();
}

inline void ThreadTeam::init ()
{
   _barrier.init( size() );
   _threadTeamData.init( _parent );
}

inline void ThreadTeam::resized ()
{
   // TODO
   _barrier.resize(size());
}

inline BaseThread & ThreadTeam::getThread ( int i ) const
{
   return *_threads[i];
}

inline BaseThread & ThreadTeam::getThread ( int i )
{
   return *_threads[i];
}

inline const BaseThread & ThreadTeam::operator[]  ( int i ) const
{
   return getThread(i);
}

inline BaseThread & ThreadTeam::operator[]  ( int i )
{
   return getThread(i);
}

inline unsigned ThreadTeam::addThread ( BaseThread *thread, bool creator )
{
   unsigned id = size();
   _threads.push_back( thread );
   if ( creator ) {
      _creatorId = (int) id;
   }
   return id;
}

inline void ThreadTeam::barrier()
{
   _barrier.barrier( myThread->getTeamId() );
}

inline ScheduleTeamData * ThreadTeam::getScheduleData() const
{
   return _scheduleData;
}

inline SchedulePolicy & ThreadTeam::getSchedulePolicy() const
{
   return _schedulePolicy;
}

inline ThreadTeamData & ThreadTeam::getThreadTeamData() const
{
   return _threadTeamData;
}

inline ThreadTeam * ThreadTeam::getParent() const
{
   return _parent;
}

inline int ThreadTeam::getLevel() const
{
   return _level;
}

inline int ThreadTeam::getCreatorId() const
{
   return _creatorId;
}

#endif
