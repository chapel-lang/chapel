
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

#ifndef _NANOS_SCHEDULE
#define _NANOS_SCHEDULE

#include <stddef.h>
#include <string>

#include "schedule_decl.hpp"
#include "workdescriptor_decl.hpp"
#include "atomic.hpp"
#include "functors.hpp"
#include <algorithm>
#include "synchronizedcondition_fwd.hpp"
#include "system_fwd.hpp"
#include "basethread_decl.hpp"

using namespace nanos;

inline bool Scheduler::checkBasicConstraints ( WD &wd, BaseThread &thread )
{
   return wd.canRunIn(*thread.runningOn()) && ( !wd.isTied() || wd.isTiedTo() == &thread );
}

inline unsigned int SchedulerConf::getNumSpins () const
{
   return _numSpins;
}

inline void SchedulerConf::setNumSpins ( const unsigned int num )
{
   _numSpins = num;
}
   
inline const std::string & SchedulePolicy::getName () const
{
   return _name;
}

inline WD * SchedulePolicy::atBeforeExit  ( BaseThread *thread, WD &current )
{
   return 0;
}

inline WD * SchedulePolicy::atAfterExit   ( BaseThread *thread, WD *current )
{
   return atIdle( thread );
}

inline WD * SchedulePolicy::atBlock       ( BaseThread *thread, WD *current )
{
   return atIdle( thread );
}

inline WD * SchedulePolicy::atYield       ( BaseThread *thread, WD *current)
{
   return atIdle( thread );
}

inline WD * SchedulePolicy::atWakeUp      ( BaseThread *thread, WD &wd )
{
   queue( thread, wd );
   return NULL;
}

inline WD * SchedulePolicy::atPrefetch    ( BaseThread *thread, WD &current )
{
   return atIdle( thread );
}

#endif

