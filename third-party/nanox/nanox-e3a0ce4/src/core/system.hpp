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

#ifndef _NANOS_SYSTEM_H
#define _NANOS_SYSTEM_H

#include "system_decl.hpp"
#include <vector>
#include <string>
#include "schedule.hpp"
#include "threadteam.hpp"
#include "slicer.hpp"
#include "nanos-int.h"
#include "dependency.hpp"
#include "instrumentation_decl.hpp"
#include "cache_map.hpp"

using namespace nanos;

// methods to access configuration variable         
inline void System::setNumPEs ( int npes ) { _numPEs = npes; }

inline int System::getNumPEs () const { return _numPEs; }

inline void System::setDeviceStackSize ( int stackSize ) { _deviceStackSize = stackSize; }

inline int System::getDeviceStackSize () const {return _deviceStackSize; }

inline void System::setBinding ( bool set ) { _bindThreads = set; }

inline bool System::getBinding () const { return _bindThreads; }

inline System::ExecutionMode System::getExecutionMode () const { return _executionMode; }

inline bool System::getVerbose () const { return _verboseMode; }

inline void System::setVerbose ( bool value ) { _verboseMode = value; }

inline void System::setInitialMode ( System::InitialMode mode ) { _initialMode = mode; }

inline System::InitialMode System::getInitialMode() const { return _initialMode; }

inline void System::setDelayedStart ( bool set) { _delayedStart = set; }

inline bool System::getDelayedStart () const { return _delayedStart; }

inline bool System::useYield() const { return _useYield; }

inline int System::getThsPerPE() const { return _thsPerPE; }

inline int System::getTaskNum() const { return _schedStats._totalTasks.value(); }

inline int System::getIdleNum() const { return _schedStats._idleThreads.value(); }

inline void System::setUntieMaster ( bool value ) { _untieMaster = value; }
inline bool System::getUntieMaster () const { return _untieMaster; }

inline void System::setSynchronizedStart ( bool value ) { _synchronizedStart = value; }
inline bool System::getSynchronizedStart ( void ) const { return _synchronizedStart; }

inline int System::getReadyNum() const { return _schedStats._readyTasks.value(); }

inline int System::getRunningTasks() const
{
   return _workers.size() - _schedStats._idleThreads.value();
}

inline int System::getNumWorkers() const { return _workers.size(); }

inline void System::setThrottlePolicy( ThrottlePolicy * policy ) { _throttlePolicy = policy; }

inline const std::string & System::getDefaultSchedule() const { return _defSchedule; }

inline const std::string & System::getDefaultThrottlePolicy() const { return _defThrottlePolicy; }

inline const std::string & System::getDefaultBarrier() const { return _defBarr; }

inline const std::string & System::getDefaultInstrumentation() const { return _defInstr; }

inline void System::setHostFactory ( peFactory factory ) { _hostFactory = factory; }

inline void System::setDefaultBarrFactory ( barrFactory factory ) { _defBarrFactory = factory; }

inline Slicer * System::getSlicer( const std::string &label ) const 
{ 
   Slicers::const_iterator it = _slicers.find(label);
   if ( it == _slicers.end() ) return NULL;
   return (*it).second;
}

inline Instrumentation * System::getInstrumentation ( void ) const { return _instrumentation; }

inline void System::setInstrumentation ( Instrumentation *instr ) { _instrumentation = instr; }

inline void System::registerSlicer ( const std::string &label, Slicer *slicer) { _slicers[label] = slicer; }

inline void System::setDefaultSchedulePolicy ( SchedulePolicy *policy ) { _defSchedulePolicy = policy; }
inline SchedulePolicy * System::getDefaultSchedulePolicy ( ) const  { return _defSchedulePolicy; }

inline SchedulerStats & System::getSchedulerStats () { return _schedStats; }
inline SchedulerConf  & System::getSchedulerConf ()  { return _schedConf; }

inline const std::string & System::getDefaultArch() const { return _defArch; }
inline void System::setDefaultArch( const std::string &arch ) { _defArch = arch; }

inline void System::setPMInterface(PMInterface *pm)
{
   ensure0(!_pmInterface,"PM interface already in place!");
   _pmInterface = pm;
}

inline const PMInterface &  System::getPMInterface(void) const { return *_pmInterface; }

inline CacheMap& System::getCacheMap() { return _cacheMap; }

inline bool System::throttleTask()
{
   return _throttlePolicy->throttle();
}

inline void System::threadReady()
{
   _initializedThreads++;
  
   /*! It's better not to call Scheduler::waitOnCondition here as the initialization is not
       yet finished 

      TODO: we can consider thread yielding */
   while (_initializedThreads.value() < _targetThreads);
}

#endif

