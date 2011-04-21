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

#include "workgroup.hpp"
#include "atomic.hpp"
#include "schedule.hpp"
#include "synchronizedcondition.hpp"
#include "system.hpp"
#include "instrumentation.hpp"
#include "workdescriptor_decl.hpp"

using namespace nanos;

Atomic<int> WorkGroup::_atomicSeed( 1 );

void WorkGroup::addWork ( WorkGroup &work )
{
   _components++;
   work.addToGroup( *this );
}

void WorkGroup::addToGroup ( WorkGroup &parent )
{
   if ( _parent == NULL ) 
      _parent = &parent;
   else 
      _partOf.push_back( &parent );
}

void WorkGroup::exitWork ( WorkGroup &work )
{
   _syncCond.reference();
   int componentsLeft = --_components;
   if (componentsLeft == 0)
      _syncCond.signal();
   _syncCond.unreference();
}

void WorkGroup::sync ()
{
   _phaseCounter++;
   //TODO: block and switch

   while ( _phaseCounter < _components );

   //TODO: reinit phase_counter
}

void WorkGroup::waitCompletion ()
{
     _syncCond.wait();
}

void WorkGroup::waitCompletionAndSignalers ()
{
     _syncCond.waitConditionAndSignalers();
}

void WorkGroup::init ()
{
}

void WorkGroup::done ()
{
   NANOS_INSTRUMENT ( static Instrumentation *instr = sys.getInstrumentation(); )

   if ( _parent != NULL ) {
     _parent->exitWork(*this);
     _parent = NULL;
     for ( WGList::iterator it = _partOf.begin(); it != _partOf.end(); it++ ) {
        if ( *it ) {
           NANOS_INSTRUMENT ( if ( ((WorkDescriptor *)(*it))->isBlocked()) { )
              NANOS_INSTRUMENT ( nanos_event_id_t id = ( ((nanos_event_id_t) getId()) << 32 ) + (*it)->getId(); )
              NANOS_INSTRUMENT ( instr->raiseOpenPtPEventNkvs ( NANOS_WAIT, id, 0, NULL, NULL );)
              NANOS_INSTRUMENT ( instr->createDeferredPtPEnd ( *((WorkDescriptor *)(*it)), NANOS_WAIT, id, 0, NULL, NULL ); )
           NANOS_INSTRUMENT ( } )
           ( *it )->exitWork( *this );
        }
        *it = 0;
     }
   }
}

WorkGroup::~WorkGroup ()
{
   done();
}

