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

#include "dependableobjectwd.hpp"
#include "workdescriptor.hpp"
#include "schedule.hpp"
#include "synchronizedcondition.hpp"
#include "smpdd.hpp"
#include "instrumentation.hpp"
#include "system.hpp"
#include "instrumentation.hpp"
#include "system.hpp"
#include "directory.hpp"

using namespace nanos;

void DOSubmit::dependenciesSatisfied ( )
{
   DependenciesDomain::decreaseTasksInGraph();
   _submittedWD->submit();
}

unsigned long DOSubmit::getDescription ( )
{
   return (unsigned long) ((nanos::ext::SMPDD &) _submittedWD->getActiveDevice()).getWorkFct();
}

void DOSubmit::instrument ( DependableObject &successor )
{
   NANOS_INSTRUMENT ( void * pred = getRelatedObject(); )
   NANOS_INSTRUMENT ( void * succ = successor.getRelatedObject(); )
   NANOS_INSTRUMENT (
                      if ( succ == NULL ) {
                         DependableObject::DependableObjectVector &succ2 = successor.getSuccessors();
                         for ( DependableObject::DependableObjectVector::iterator it = succ2.begin(); it != succ2.end(); it++ ) {
                            instrument ( *(*it) ); 
                         }
                         return;
                      }
                    )
   NANOS_INSTRUMENT ( static Instrumentation *instr = sys.getInstrumentation(); )
   NANOS_INSTRUMENT ( WorkDescriptor *wd_sender = (WorkDescriptor *) pred; )
   NANOS_INSTRUMENT ( WorkDescriptor *wd_receiver = (WorkDescriptor *) succ; )
   NANOS_INSTRUMENT ( nanos_event_id_t id = ( ((nanos_event_id_t) wd_sender->getId()) << 32 ) + wd_receiver->getId(); )
   NANOS_INSTRUMENT ( instr->raiseOpenPtPEventNkvs( NANOS_WD_DEPENDENCY, id, 0, NULL, NULL ); )
   NANOS_INSTRUMENT ( instr->createDeferredPtPEnd ( *wd_receiver, NANOS_WD_DEPENDENCY, id, 0, NULL, NULL ); )
}


bool DOWait::waits()
{
   return true;
}

void DOWait::init()
{
   _depsSatisfied = false;
}

void DOWait::wait ( std::list<Dependency *> deps )
{
   _syncCond.wait();

   Directory *d = _waitDomainWD->getDirectory(false);
   if ( d != NULL ) {
      std::list<uint64_t> flushDeps;
      for (std::list<Dependency *>::iterator it = deps.begin(); it != deps.end(); it++ ) {
         flushDeps.push_back( (uint64_t) (*it)->getDepAddress() );
      }
      d->synchronizeHost( flushDeps );
   }
}

void DOWait::dependenciesSatisfied ( )
{
   DependenciesDomain::decreaseTasksInGraph();
   _depsSatisfied = true;
   _syncCond.signal();
}

void DOWait::instrument ( DependableObject &successor )
{
   NANOS_INSTRUMENT ( void * pred = getRelatedObject(); )
   NANOS_INSTRUMENT ( void * succ = successor.getRelatedObject(); )
   NANOS_INSTRUMENT (
                      if ( succ == NULL ) {
                         DependableObject::DependableObjectVector &succ2 = successor.getSuccessors();
                         for ( DependableObject::DependableObjectVector::iterator it = succ2.begin(); it != succ2.end(); it++ ) {
                            instrument ( *(*it) ); 
                         }
                         return;
                      }
                    )
   NANOS_INSTRUMENT ( static Instrumentation *instr = sys.getInstrumentation(); )
   NANOS_INSTRUMENT ( WorkDescriptor *wd_sender = (WorkDescriptor *) pred; )
   NANOS_INSTRUMENT ( WorkDescriptor *wd_receiver = (WorkDescriptor *) succ; )
   NANOS_INSTRUMENT ( nanos_event_id_t id = ( ((nanos_event_id_t) wd_sender->getId()) << 32 ) + wd_receiver->getId(); )
   NANOS_INSTRUMENT ( instr->raiseOpenPtPEventNkvs( NANOS_WD_DEPENDENCY, id, 0, NULL, NULL ); )
   NANOS_INSTRUMENT ( instr->createDeferredPtPEnd ( *wd_receiver, NANOS_WD_DEPENDENCY, id, 0, NULL, NULL ); )
}

