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
#include "instrumentation.hpp"

#include "instrumentationcontext.hpp"
#include "system.hpp"
#include "compatibility.hpp"
#include "workdescriptor.hpp"
#include <alloca.h>

using namespace nanos;

#ifdef NANOS_INSTRUMENTATION_ENABLED

/* ************************************************************************** */
/* ***                   C R E A T I N G   E V E N T S                    *** */
/* ************************************************************************** */
void Instrumentation::createStateEvent( Event *e, nanos_event_state_value_t state )
{
   /* Registering a state event in instrucmentor context */
   InstrumentationContextData *icd = myThread->getCurrentWD()->getInstrumentationContextData();
   _instrumentationContext.pushState(icd, state);

   /* Creating a state event */
   if ( _instrumentationContext.isStateEventEnabled( icd ) ) new (e) State(NANOS_STATE_START, state);
   else new (e) State(NANOS_SUBSTATE_START, state);
}

void Instrumentation::returnPreviousStateEvent ( Event *e )
{
   /* Recovering a state event in instrumentation context */
   InstrumentationContextData  *icd = myThread->getCurrentWD()->getInstrumentationContextData();
   _instrumentationContext.popState( icd );
   nanos_event_state_value_t state = _instrumentationContext.topState( icd ); 

   /* Creating a state event */
   if ( _instrumentationContext.isStateEventEnabled( icd ) ) new (e) State(NANOS_STATE_END,state);
   else new (e) State(NANOS_SUBSTATE_END, state);
}

void Instrumentation::createBurstEvent ( Event *e, nanos_event_key_t key, nanos_event_value_t value, InstrumentationContextData *icd )
{
   /* Recovering a state event in instrumentation context */
   if ( icd == NULL ) icd = myThread->getCurrentWD()->getInstrumentationContextData();

   /* Creating burst  event */
   Event::KV *kv = NEW Event::KV( key, value );
   new (e) Burst( true, kv );

   _instrumentationContext.insertBurst( icd, *e );
}

void Instrumentation::closeBurstEvent ( Event *e, nanos_event_key_t key, InstrumentationContextData *icd )
{
   /* Recovering a state event in instrumentation context */
   if ( icd == NULL ) icd = myThread->getCurrentWD()->getInstrumentationContextData();

   InstrumentationContextData::BurstIterator it;

   /* find given key in the burst list */
   if ( _instrumentationContext.findBurstByKey( icd, key, it ) ) {
      /* Creating burst event */
      new (e) Event(*it);
      e->reverseType();
      _instrumentationContext.removeBurst( icd, it ); 
   }
   else fatal("Burst type doesn't exists");

   /* If not needed to show stacked bursts then close current event by openning next one (if any)  */
   if ( ( !_instrumentationContext.showStackedBursts()) && (_instrumentationContext.findBurstByKey( icd, key, it )) ) {
      new (e) Event(*it);
   }
}

void Instrumentation::createPointEvent ( Event *e, unsigned int nkvs, nanos_event_key_t *keys,
                                      nanos_event_value_t *values )
{
   /* Creating an Event::KV vector */
   Event::KVList kvlist = NEW Event::KV[nkvs];

   /* Initializing kvlist elements */
   for ( unsigned int i = 0; i < nkvs; i++ ) {
      kvlist[i] = Event::KV ( keys[i], values[i] );
   }

   /* Creating a point event */
   new (e) Point ( nkvs, kvlist );

}

void Instrumentation::createPtPStart ( Event *e, nanos_event_domain_t domain, nanos_event_id_t id,
                      unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values )
{
   /* Creating an Event::KV vector */
   Event::KVList kvlist = NEW Event::KV[nkvs];

   /* Initializing kvlist elements */
   for ( unsigned int i = 0; i < nkvs; i++ ) {
      kvlist[i] = Event::KV ( keys[i], values[i] );
   }

   /* Creating a PtP (start) event */
   new (e) PtP ( true, domain, id, nkvs, kvlist );
}

void Instrumentation::createPtPEnd ( Event *e, nanos_event_domain_t domain, nanos_event_id_t id,
                      unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values )
{
   /* Creating an Event::KV vector */
   Event::KVList kvlist = NEW Event::KV[nkvs];

   /* Initializing kvlist elements */
   for ( unsigned int i = 0; i < nkvs; i++ ) {
      kvlist[i] = Event::KV ( keys[i], values[i] );
   }

   /* Creating a PtP (end) event */
   new (e) PtP ( false, domain, id, nkvs, kvlist );

}
/* ************************************************************************** */
/* ***          C R E A T I N G   D E F E R R E D   E V E N T S           *** */
/* ************************************************************************** */

void Instrumentation::createDeferredPointEvent ( WorkDescriptor &wd, unsigned int nkvs, nanos_event_key_t *keys,
                                      nanos_event_value_t *values )
{
   Event e; /* Event */

   /* Create point event */
   createPointEvent ( &e, nkvs, keys, values );
   
   /* Inserting event into deferred event list */
   InstrumentationContextData *icd = wd.getInstrumentationContextData();                                             
   _instrumentationContext.insertDeferredEvent( icd, e );

}

void Instrumentation::createDeferredPtPStart ( WorkDescriptor &wd, nanos_event_domain_t domain, nanos_event_id_t id,
                      unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values )
{
   Event e; /* Event */

   /* Create event: PtP */
   createPtPStart( &e, domain, id, nkvs, keys, values );

   /* Inserting event into deferred event list */
   InstrumentationContextData *icd = wd.getInstrumentationContextData();                                             
   _instrumentationContext.insertDeferredEvent( icd, e );
}

void Instrumentation::createDeferredPtPEnd ( WorkDescriptor &wd, nanos_event_domain_t domain, nanos_event_id_t id,
                      unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values )
{
   Event e; /* Event */

   /* Create event: PtP */
   createPtPEnd( &e, domain, id, nkvs, keys, values );

   /* Inserting event into deferred event list */
   InstrumentationContextData *icd = wd.getInstrumentationContextData();                                             
   _instrumentationContext.insertDeferredEvent( icd, e );
}
/* ************************************************************************** */
/* ***                   T H R O W I N G   E V E N T S                    *** */
/* ************************************************************************** */

void Instrumentation::raisePointEvent ( nanos_event_key_t key, nanos_event_value_t val )
{
   Event e; /* Event */

   /* Create point event */
   createPointEvent ( &e, 1, &key, &val );

   /* Spawning point event */
   addEventList ( 1, &e );
}

void Instrumentation::raisePointEventNkvs ( unsigned int nkvs, nanos_event_key_t *key, nanos_event_value_t *val )
{
   Event e; /* Event */

   /* Create point event */
   createPointEvent ( &e, nkvs, key, val );

   /* Spawning point event */
   addEventList ( 1, &e );
}

void Instrumentation::raiseOpenStateEvent ( nanos_event_state_value_t state )
{
   Event e; /* Event */

   /* Create state event */
   createStateEvent( &e, state );

   /* Spawning state event */
   addEventList ( 1, &e );
}

void Instrumentation::raiseCloseStateEvent ( void )
{
   Event e; /* Event */

   /* Create state event */
   returnPreviousStateEvent( &e );

   /* Spawning state event */
   addEventList ( 1, &e );

}

void Instrumentation::raiseOpenBurstEvent ( nanos_event_key_t key, nanos_event_value_t val )
{
   Event e; /* Event */

   /* Create event: BURST */
   createBurstEvent( &e, key, val );

   /* Spawning event: specific instrumentation call */
   addEventList ( 1, &e );
}

void Instrumentation::raiseCloseBurstEvent ( nanos_event_key_t key )
{
   Event e; /* Event */

   /* Create event: BURST */
   closeBurstEvent( &e, key );

   /* Spawning event: specific instrumentation call */
   addEventList ( 1, &e );
}

void Instrumentation::raiseOpenPtPEvent ( nanos_event_domain_t domain, nanos_event_id_t id, nanos_event_key_t key, nanos_event_value_t val )
{
   Event e; /* Event */

   /* Create event: PtP */
   createPtPStart( &e, domain, id, 1, &key, &val );

   /* Spawning event: specific instrumentation call */
   addEventList ( 1, &e );
}

void Instrumentation::raiseOpenPtPEventNkvs ( nanos_event_domain_t domain, nanos_event_id_t id, unsigned int nkvs,
                                           nanos_event_key_t *key, nanos_event_value_t *val )
{
   Event e; /* Event */

   /* Create event: PtP */
   createPtPStart( &e, domain, id, nkvs, key, val );

   /* Spawning event: specific instrumentation call */
   addEventList ( 1, &e );
}

void Instrumentation::raiseClosePtPEvent ( nanos_event_domain_t domain, nanos_event_id_t id, nanos_event_key_t key, nanos_event_value_t val )
{
   Event e; /* Event */

   /* Create event: PtP */
   createPtPEnd( &e, domain, id, 1, &key, &val );

   /* Spawning event: specific instrumentation call */
   addEventList ( 1, &e );

}

void Instrumentation::raiseClosePtPEventNkvs ( nanos_event_domain_t domain, nanos_event_id_t id, unsigned int nkvs,
                                            nanos_event_key_t *key, nanos_event_value_t *val )
{
   Event e; /* Event */

   /* Create event: PtP */
   createPtPEnd( &e, domain, id, nkvs, key, val );

   /* Spawning event: specific instrumentation call */
   addEventList ( 1, &e );

}

void Instrumentation::raiseOpenStateAndBurst ( nanos_event_state_value_t state, nanos_event_key_t key, nanos_event_value_t val )
{
   Event e[2]; /* Event array */

   /* Create a vector of two events: STATE and BURST */
   createStateEvent( &e[0], state );
   createBurstEvent( &e[1], key, val );

   /* Spawning two events: specific instrumentation call */
   addEventList ( 2, e );

}

void Instrumentation::raiseCloseStateAndBurst ( nanos_event_key_t key )
{
   Event e[2]; /* Event array */

   /* Creating a vector of two events: STATE and BURST */
   returnPreviousStateEvent( &e[0] );
   closeBurstEvent( &e[1], key ); 
 
   /* Spawning two events: specific instrumentation call */
   addEventList ( 2, e );
}

/* ************************************************************************** */
/* ***            C O N T E X T   S W I T C H    E V E N T S              *** */
/* ************************************************************************** */

void Instrumentation::wdCreate( WorkDescriptor* newWD )
{
   Event e; /* Event */

   /* Gets key for wd-id bursts and wd->id as value*/
   static nanos_event_key_t key = getInstrumentationDictionary()->getEventKey("wd-id");
   nanos_event_value_t wd_id = newWD->getId();
   
   /* Update InstrumentationContextData */
   InstrumentationContextData *icd = newWD->getInstrumentationContextData();
   _instrumentationContext.pushState( icd, NANOS_RUNTIME );
   
   /* Create event: BURST */
   createBurstEvent( &e, key, wd_id, icd );

}

void Instrumentation::wdSwitch( WorkDescriptor* oldWD, WorkDescriptor* newWD, bool last )
{
   unsigned int i = 0;
   unsigned int oldPtP = 0, oldStates = 0, oldSubStates = 0, oldBursts = 0;
   unsigned int newPtP = 0, newStates = 0, newSubStates = 0, newBursts = 0, newDeferred = 0;
   InstrumentationContextData *old_icd = NULL;
   InstrumentationContextData *new_icd = NULL;


   /* Computing number of leaving wd related events*/
   if ( oldWD!=NULL ) {
      /* Getting Instrumentation Context */
      old_icd = oldWD->getInstrumentationContextData();

      oldPtP = last ? 0 : 1;
      if ( _instrumentationContext.showStackedStates () ) {
         oldStates = _instrumentationContext.getStateStackSize(old_icd) + 1;
      }
      else oldStates = 1;
      if ( !_instrumentationContext.isStateEventEnabled ( old_icd ) ) {
         if ( _instrumentationContext.showStackedStates() ) {
            oldSubStates = _instrumentationContext.getSubStateStackSize(old_icd) + 1;
         }
         else oldSubStates = 1;
      }
      else oldSubStates = 0;
      oldBursts = _instrumentationContext.getNumBursts( old_icd );
   }

   /* Computing number of entering wd related events*/
   if ( newWD!=NULL ) {
      /* Getting Instrumentation Context */
      new_icd = newWD->getInstrumentationContextData();

      newPtP = 1;
      if ( _instrumentationContext.showStackedStates () ) {
         newStates = _instrumentationContext.getStateStackSize(new_icd);
         if ( !(new_icd->getStartingWD()) ) newStates++;
      }
      else newStates = 1;
      if ( !_instrumentationContext.isStateEventEnabled ( new_icd ) ) {
         if ( _instrumentationContext.showStackedStates() ) newSubStates = _instrumentationContext.getSubStateStackSize(new_icd) + 1;
         else newSubStates = 1;
      }
      else newSubStates = 0;
      newBursts = _instrumentationContext.getNumBursts( new_icd );
      newDeferred = _instrumentationContext.getNumDeferredEvents ( new_icd );
   }

   /* Allocating Events */
   unsigned int numEvents = oldPtP + oldStates + oldSubStates + oldBursts
                          + newPtP + newStates + newSubStates + newBursts + newDeferred;

   Event *e = (Event *) alloca(sizeof(Event) * numEvents );

   /* Creating leaving wd events */
   if ( old_icd!= NULL ) {
      /* Creating a starting PtP event (if needed) */
      if (!last) ASSIGN_EVENT( e[i++] , PtP , (true,  NANOS_WD_DOMAIN, (nanos_event_id_t) oldWD->getId(), 0, NULL) );

      /* Creating State event's */
      InstrumentationContextData::ConstStateIterator it_s;

      /* Creating State event's: states */
      if ( _instrumentationContext.showStackedStates() ) {
         for ( it_s = _instrumentationContext.beginState( old_icd ); it_s != _instrumentationContext.endState( old_icd ); it_s++ ) {
	     ASSIGN_EVENT( e[i++] ,  State , (NANOS_STATE_END, *it_s) );
         }
      }

      /* In both cases (showStackedStates or not) keep 'current state' as RUNTIME */
      ASSIGN_EVENT( e[i++] , State , ( NANOS_STATE_START, NANOS_RUNTIME ) );

      /* Creating State event's: substates */
      if ( !_instrumentationContext.isStateEventEnabled( old_icd ) ) {
         if ( _instrumentationContext.showStackedStates () ) {
            for ( it_s = _instrumentationContext.beginSubState(old_icd); it_s != _instrumentationContext.endSubState(old_icd); it_s++ ) {
		ASSIGN_EVENT( e[i++] , State , ( NANOS_SUBSTATE_END, *it_s ) );
            }
         }
         ASSIGN_EVENT( e[i++] , State , ( NANOS_SUBSTATE_START, NANOS_NOT_TRACED ) );
      }

      /* Regenerating reverse bursts for old WD */
      InstrumentationContextData::ConstBurstIterator it;
      for ( it = _instrumentationContext.beginBurst( old_icd ); it != _instrumentationContext.endBurst( old_icd ); it++ ) {
         e[i] = *it; e[i++].reverseType();
      }

   }

   /* Creating entering wd events */
   if ( new_icd!=NULL) {
      /* Creating PtP event */
      ASSIGN_EVENT( e[i++] , PtP , (false, NANOS_WD_DOMAIN, (nanos_event_id_t) newWD->getId(), 0, NULL) );

      /* Creating State event's */
      InstrumentationContextData::ConstStateIterator it_s;
      nanos_event_state_value_t state;

      /* Creating State event's: states */
      if ( _instrumentationContext.showStackedStates() ) {

         /* Close current state (+1) when computing number of states (only if not a startingWD) */
         if ( new_icd->getStartingWD() == false ) ASSIGN_EVENT( e[i++] , State , ( NANOS_STATE_END, NANOS_RUNTIME ) );
         else new_icd->setStartingWD(false);

         for ( it_s = _instrumentationContext.beginState( new_icd ); it_s != _instrumentationContext.endState( new_icd ); it_s++) {
            ASSIGN_EVENT( e[i++] , State , ( NANOS_STATE_START, *it_s ) );
         }
      } else {
         state = _instrumentationContext.getState( new_icd );
         ASSIGN_EVENT( e[i++] , State , ( NANOS_STATE_START, state ) );
      }

      /* Creating State event's: substates */
      if ( !_instrumentationContext.isStateEventEnabled( new_icd ) ) {
         if ( _instrumentationContext.showStackedStates () ) {
            ASSIGN_EVENT( e[i++] , State , ( NANOS_SUBSTATE_END, NANOS_NOT_TRACED ) );
            for ( it_s = _instrumentationContext.beginSubState(new_icd); it_s != _instrumentationContext.endSubState(new_icd); it_s++ ) {
                ASSIGN_EVENT( e[i++] , State , ( NANOS_SUBSTATE_START, *it_s ) );
            }
         } else {
            state = _instrumentationContext.getSubState( new_icd );
            ASSIGN_EVENT( e[i++] , State , ( NANOS_SUBSTATE_START, state ) );
         }
      }

      /* Regenerating bursts for new WD: in reverse order */
      InstrumentationContextData::ConstBurstIterator it;
      i += (newBursts-1);
      for ( it = _instrumentationContext.beginBurst( new_icd ) ; it != _instrumentationContext.endBurst( new_icd ); it++ ) {
         e[i--] = *it;
      }
      i += (newBursts+1);

      /* Generating deferred events for new WD (and removing them) */
      InstrumentationContextData::EventIterator itDE;
      for ( itDE  = _instrumentationContext.beginDeferredEvents( new_icd );
            itDE != _instrumentationContext.endDeferredEvents(new_icd); itDE++ ) {
         e[i++] = *itDE;
      }
      _instrumentationContext.clearDeferredEvents( new_icd );
   }
   
   ensure0( i == numEvents , "Computed number of events doesn't fit with number of real events");

   /* Spawning 'numEvents' events: specific instrumentation call */
   addEventList ( numEvents, e );

   /* Calling array event's destructor: cleaning events */
   for ( i = 0; i <numEvents; i++ ) e[i].~Event();
}

void Instrumentation::enableStateEvents()
{
   /* Closing user's defined state: coherent state should be NOT_TRACED */
   raiseCloseStateEvent();

   /* Getting Instrumentation Context Data */
   InstrumentationContextData *icd = myThread->getCurrentWD()->getInstrumentationContextData();

   /* Enabling state instrumentation */
   _instrumentationContext.enableStateEvents( icd );
}

void Instrumentation::disableStateEvents( nanos_event_state_value_t state )
{
   /* Getting Instrumentation Context Data */
   InstrumentationContextData *icd = myThread->getCurrentWD()->getInstrumentationContextData();

   /* Disabling state instrumentation */
   _instrumentationContext.disableStateEvents( icd );

   Event e[2]; /* Event array */

   /* Creating a vector of two stata events: NOT_TRACED & RUNTIME */
   createStateEvent( &e[0], NANOS_NOT_TRACED );
   createStateEvent( &e[1], state );
 
   /* Spawning two events: specific instrumentation call */
   addEventList ( 2, e );
}

#endif
