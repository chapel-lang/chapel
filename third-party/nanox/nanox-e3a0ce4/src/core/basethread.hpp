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

#ifndef _BASE_THREAD_ELEMENT
#define _BASE_THREAD_ELEMENT

#include "workdescriptor_fwd.hpp"
#include "atomic.hpp"
#include "processingelement.hpp"
#include "debug.hpp"
#include "schedule_fwd.hpp"
#include "threadteam_fwd.hpp"
#include "basethread_decl.hpp"
#include "atomic.hpp"

namespace nanos
{
   // atomic access
   inline void BaseThread::lock () { _mlock++; }
 
   inline void BaseThread::unlock () { _mlock--; }
 
   inline void BaseThread::stop() { _mustStop = true; }
 
   // set/get methods
   inline void BaseThread::setCurrentWD ( WD &current ) { _currentWD = &current; }
 
   inline WD * BaseThread::getCurrentWD () const { return _currentWD; }
 
   inline WD & BaseThread::getThreadWD () const { return _threadWD; }
 
   inline void BaseThread::resetNextWD () { _nextWD = NULL; }
 
   inline bool BaseThread::setNextWD ( WD *next ) { 
      return compareAndSwap( &_nextWD, (WD *) NULL, next);
   }
 
   inline WD * BaseThread::getNextWD () const { return _nextWD; }
 
   // team related methods
   inline void BaseThread::reserve() { _hasTeam = 1; }
 
   inline void BaseThread::enterTeam( ThreadTeam *newTeam, TeamData *data ) 
   {
      _teamData = data;
      memoryFence();
      _team = newTeam;
      _hasTeam=1;
   }
 
   inline bool BaseThread::hasTeam() const { return _hasTeam; }
 
   inline void BaseThread::leaveTeam() { _hasTeam = 0; _team = 0; }
 
   inline ThreadTeam * BaseThread::getTeam() const { return _team; }
 
   inline TeamData * BaseThread::getTeamData() const { return _teamData; }
 
   //! Returns the id of the thread inside its current team 
   inline int BaseThread::getTeamId() const { return _teamData->getId(); }
 
   inline bool BaseThread::isStarted () const { return _started; }
 
   inline bool BaseThread::isRunning () const { return _started && !_mustStop; }
 
   inline ProcessingElement * BaseThread::runningOn() const { return _pe; }
 
   inline int BaseThread::getId() { return _id; }
 
   inline int BaseThread::getCpuId() { return runningOn()->getId(); }
 
   inline Allocator & BaseThread::getAllocator() { return _allocator; }
   /*! \brief Rename the basethread
   */
   inline void BaseThread::rename ( const char *name )
   {
     _name = name;
   }
 
   /*! \brief Get BaseThread name
   */
   inline const std::string & BaseThread::getName ( void ) const
   {
      return _name;
   }
 
   /*! \brief Get BaseThread description
   */
   inline const std::string & BaseThread::getDescription ( void ) 
   {
     if ( _description.empty() ) {
 
        /* description name */
        _description = getName();
        _description.append("-");
 
        /* adding device type */
        _description.append( _pe->getDeviceType().getName() );
        _description.append("-");
 
        /* adding global id */
        _description.append( toString<int>(getId()) );
     }
 
     return _description;
   }

}

#endif
