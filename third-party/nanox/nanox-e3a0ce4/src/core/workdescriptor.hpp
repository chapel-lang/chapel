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

#ifndef _NANOS_WORK_DESCRIPTOR_H
#define _NANOS_WORK_DESCRIPTOR_H

#include <stdlib.h>
#include <utility>
#include <vector>
#include "workdescriptor_decl.hpp"
#include "workgroup.hpp"
#include "dependableobjectwd.hpp"
#include "copydata.hpp"
#include "synchronizedcondition_decl.hpp"
#include "atomic.hpp"
#include "lazy.hpp"
#include "instrumentationcontext.hpp"
#include "directory.hpp"
#include "schedule.hpp"
#include "dependenciesdomain.hpp"
#include "allocator_decl.hpp"

using namespace nanos;

/* DeviceData inlined functions */
inline bool DeviceData::isCompatible ( const Device &arch ) { return _architecture == &arch; }

/* WorkDescriptor inlined functions */
inline bool WorkDescriptor::started ( void ) const { return _state != INIT; }

inline size_t WorkDescriptor::getDataSize () const { return _data_size; }
inline void WorkDescriptor::setDataSize ( size_t data_size ) { _data_size = data_size; }

inline int WorkDescriptor::getDataAlignment () const { return _data_align; }
inline void WorkDescriptor::setDataAlignment ( int data_align ) { _data_align = data_align; }

inline WorkDescriptor * WorkDescriptor::getParent() { return _parent; }
inline void WorkDescriptor::setParent ( WorkDescriptor * p ) { _parent = p; }

inline WDDeque * WorkDescriptor::getMyQueue() { return _myQueue; }
inline void WorkDescriptor::setMyQueue ( WDDeque * myQ ) { _myQueue = myQ; }

inline bool WorkDescriptor::isEnqueued() { return ( _myQueue != NULL ); }

inline WorkDescriptor & WorkDescriptor::tied () { _tie = true; return *this; }

inline WorkDescriptor & WorkDescriptor::tieTo ( BaseThread &pe ) { _tiedTo = &pe; _tie=false; return *this; }

inline bool WorkDescriptor::isTied() const { return _tiedTo != NULL; }

inline BaseThread* WorkDescriptor::isTiedTo() const { return _tiedTo; }

inline void WorkDescriptor::setData ( void *wdata ) { _data = wdata; }

inline void * WorkDescriptor::getData () const { return _data; }

inline bool WorkDescriptor::isIdle () const { return _state == WorkDescriptor::IDLE; }
inline void WorkDescriptor::setIdle () { _state = WorkDescriptor::IDLE; }

inline bool WorkDescriptor::isBlocked () const { return _state == WorkDescriptor::BLOCKED; }
inline void WorkDescriptor::setBlocked () { _state = WorkDescriptor::BLOCKED; }

inline bool WorkDescriptor::isReady () const { return _state == WorkDescriptor::READY; }
inline void WorkDescriptor::setReady () { _state = WorkDescriptor::READY; }

inline GenericSyncCond * WorkDescriptor::getSyncCond() { return _syncCond; }

inline void WorkDescriptor::setSyncCond( GenericSyncCond * syncCond ) { _syncCond = syncCond; }

inline void WorkDescriptor::setDepth ( int l ) { _depth = l; }

inline unsigned WorkDescriptor::getDepth() { return _depth; }

inline DeviceData & WorkDescriptor::getActiveDevice () const { return *_activeDevice; }

inline bool WorkDescriptor::hasActiveDevice() const { return _activeDevice != NULL; }

inline void WorkDescriptor::setInternalData ( void *data ) { _wdData = data; }

inline void * WorkDescriptor::getInternalData () const { return _wdData; }

inline void WorkDescriptor::setTranslateArgs( nanos_translate_args_t translateArgs ) { _translateArgs = translateArgs; }

inline unsigned int WorkDescriptor::getNumDevices ( void ) { return _numDevices; }

inline DeviceData ** WorkDescriptor::getDevices ( void ) { return _devices; }

inline bool WorkDescriptor::dequeue ( WorkDescriptor **slice ) { *slice = this; return true; }

inline void WorkDescriptor::clear () { _parent = NULL; }

inline size_t WorkDescriptor::getNumCopies() const { return _numCopies; }

inline CopyData * WorkDescriptor::getCopies() const { return _copies; }

inline TR1::shared_ptr<DOSubmit> & WorkDescriptor::getDOSubmit() { return _doSubmit; }

inline void WorkDescriptor::submitWithDependencies( WorkDescriptor &wd, size_t numDeps, Dependency* deps )
{
   wd._doSubmit.reset( NEW DOSubmit() );
   wd._doSubmit->setWD(&wd);
   _depsDomain->submitDependableObject( *(wd._doSubmit), numDeps, deps );
}

inline void WorkDescriptor::waitOn( size_t numDeps, Dependency* deps )
{
   _doWait->setWD(this);
   _depsDomain->submitDependableObject( *_doWait, numDeps, deps );
}

class DOIsSchedulable : public DependableObjectPredicate
{
   BaseThread &    _thread;

   public:
      DOIsSchedulable(BaseThread &thread) : DependableObjectPredicate(),_thread(thread) { }
      ~DOIsSchedulable() {}

      bool operator() ( DependableObject &obj )
      {       
         WD *wd = (WD *)obj.getRelatedObject();
         // FIXME: The started condition here ensures that doWait objects are not released as
         // they do not work properly if there is no dependenceSatisfied called before
         return (wd != NULL) && Scheduler::checkBasicConstraints(*wd,_thread) && !wd->started() ;
      }
};

inline WorkDescriptor * WorkDescriptor::getImmediateSuccessor ( BaseThread &thread )
{
   if ( _doSubmit == NULL ) return NULL;
   else {
        DOIsSchedulable predicate(thread);
        DependableObject * found = _doSubmit->releaseImmediateSuccessor(predicate);
        return found ? (WD *) found->getRelatedObject() : NULL;
   }
}

inline void WorkDescriptor::workFinished(WorkDescriptor &wd)
{
   if ( wd._doSubmit != NULL )
      wd._doSubmit->finished();
}

inline DependenciesDomain & WorkDescriptor::getDependenciesDomain()
{
   return *_depsDomain;
}


inline InstrumentationContextData * WorkDescriptor::getInstrumentationContextData( void ) { return &_instrumentationContextData; }

inline void WorkDescriptor::waitCompletion()
{
   this->WorkGroup::waitCompletion();
   if ( _directory.isInitialized() )
      _directory->synchronizeHost();
}

inline void WorkDescriptor::waitCompletionAndSignalers()
{
   this->WorkGroup::waitCompletionAndSignalers();
   if ( _directory.isInitialized() )
      _directory->synchronizeHost();
}

inline Directory* WorkDescriptor::getDirectory(bool create)
{
   if ( !_directory.isInitialized() && create == false ) {
      return NULL;
   }
   _directory->setParent( (getParent() != NULL) ? getParent()->getDirectory(false) : NULL );
   return &(*_directory);
}

inline bool WorkDescriptor::isSubmitted() const { return _submitted; }
inline void WorkDescriptor::submitted()  { _submitted = true; }

#endif

