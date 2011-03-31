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
#include <stack>
#include <list>

#include "instrumentationcontext.hpp"
#include "instrumentation.hpp"
#include "debug.hpp"

using namespace nanos;

#ifdef NANOS_INSTRUMENTATION_ENABLED

bool InstrumentationContext::showStackedBursts ( void ) { return false; }
bool InstrumentationContext::showStackedStates ( void ) { return false; }

bool InstrumentationContextStackedStates::showStackedBursts ( void ) { return false; }
bool InstrumentationContextStackedStates::showStackedStates ( void ) { return true; }

bool InstrumentationContextStackedBursts::showStackedBursts ( void ) { return true; }
bool InstrumentationContextStackedBursts::showStackedStates ( void ) { return false; }

bool InstrumentationContextStackedStatesAndBursts::showStackedBursts ( void ) { return true; }
bool InstrumentationContextStackedStatesAndBursts::showStackedStates ( void ) { return true; }


void InstrumentationContext::insertBurst ( InstrumentationContextData *icd, const Event &e )
{
   bool found = false;
   InstrumentationContextData::EventList::iterator it;
   nanos_event_key_t key = e.getKVs()[0].first;

   /* if found an event with the same key in the main list, send it to the backup list */
   for ( it = icd->_burstList.begin() ; !found && (it != icd->_burstList.end()) ; it++ ) {
      Event::ConstKVList kvlist = (*it).getKVs();
      if ( kvlist[0].first == key  )
      {
         icd->_burstBackup.splice ( icd->_burstBackup.begin(), icd->_burstList, it );
         found = true;
      }
   }

   /* insert the event into the list */
   icd->_burstList.push_front ( e );

}

void InstrumentationContext::removeBurst ( InstrumentationContextData *icd, InstrumentationContextData::BurstIterator it )
{
   bool found = false;
   nanos_event_key_t key = (*it).getKVs()[0].first;

   /* remove event from the list */
   icd->_burstList.erase ( it );

   /* if found an event with the same key in the backup list, recover it to the main list */
   for ( it = icd->_burstBackup.begin() ; !found && (it != icd->_burstBackup.end()) ; it++ ) {
      Event::ConstKVList kvlist = (*it).getKVs();
      if ( kvlist[0].first == key  )
      {
         icd->_burstList.splice ( icd->_burstList.begin(), icd->_burstBackup, it );
         found = true;
      }
   }
}

void InstrumentationContextStackedBursts::insertBurst ( InstrumentationContextData *icd, const Event &e )
{
   /* insert the event into the list */
   icd->_burstList.push_front ( e );
}

void InstrumentationContextStackedBursts::removeBurst ( InstrumentationContextData *icd, InstrumentationContextData::BurstIterator it )
{
   /* remove event from the list */
   icd->_burstList.erase ( it );
}

void InstrumentationContextStackedStatesAndBursts::insertBurst ( InstrumentationContextData *icd, const Event &e )
{
   /* insert the event into the list */
   icd->_burstList.push_front ( e );
}

void InstrumentationContextStackedStatesAndBursts::removeBurst ( InstrumentationContextData *icd, InstrumentationContextData::BurstIterator it )
{
   /* remove event from the list */
   icd->_burstList.erase ( it );
}

#endif
