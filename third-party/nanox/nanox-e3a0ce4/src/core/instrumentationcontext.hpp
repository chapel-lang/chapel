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
#ifndef __NANOS_INSTRUMENTOR_CTX_H
#define __NANOS_INSTRUMENTOR_CTX_H
#include <stack>
#include <list>

#include "instrumentationcontext_decl.hpp"
#include "debug.hpp"

using namespace nanos;

#ifdef NANOS_INSTRUMENTATION_ENABLED

inline void InstrumentationContext::pushState ( InstrumentationContextData *icd, nanos_event_state_value_t state )
{
   if ( icd->_stateEventEnabled ) icd->_stateStack.push_back( state );
   else icd->_subStateStack.push_back ( state );
}

inline void InstrumentationContext::popState ( InstrumentationContextData *icd )
{
   if ( (icd->_stateEventEnabled ) && !(icd->_stateStack.empty()) ) icd->_stateStack.pop_back();
   else if ( !(icd->_subStateStack.empty()) ) icd->_subStateStack.pop_back();
}

inline nanos_event_state_value_t InstrumentationContext::topState ( InstrumentationContextData *icd )
{
   if ( (icd->_stateEventEnabled) && !(icd->_stateStack.empty()) ) return icd->_stateStack.back();
   else if ( !(icd->_subStateStack.empty()) ) return icd->_subStateStack.back();
   else return NANOS_ERROR;
}

inline nanos_event_state_value_t InstrumentationContext::getState ( InstrumentationContextData *icd )
{
   if ( !(icd->_stateStack.empty()) ) return icd->_stateStack.back();
   else return NANOS_ERROR;
}

inline nanos_event_state_value_t InstrumentationContext::getSubState ( InstrumentationContextData *icd )
{
   if ( !(icd->_subStateStack.empty()) ) return icd->_subStateStack.back();
   else return NANOS_ERROR;
}

inline size_t InstrumentationContext::getStateStackSize ( InstrumentationContextData *icd )
{
   return (size_t) icd->_stateStack.size();
}

inline size_t InstrumentationContext::getSubStateStackSize ( InstrumentationContextData *icd )
{
   return (size_t) icd->_subStateStack.size();
}

inline bool InstrumentationContext::findBurstByKey ( InstrumentationContextData *icd, nanos_event_key_t key,
                                                     InstrumentationContextData::BurstIterator &ret )
{
   bool found = false;
   InstrumentationContextData::BurstIterator it;

   for ( it = icd->_burstList.begin() ; !found && (it != icd->_burstList.end()) ; it++ ) {
      Instrumentation::Event::ConstKVList kvlist = (*it).getKVs();
      if ( kvlist[0].first == key  ) { ret = it; found = true;}
   }

   return found;
}

inline size_t InstrumentationContext::getNumBursts( InstrumentationContextData *icd ) const
{
   return icd->_burstList.size();
}

inline InstrumentationContextData::ConstBurstIterator InstrumentationContext::beginBurst( InstrumentationContextData *icd ) const
{
   return icd->_burstList.begin();
}

inline InstrumentationContextData::ConstBurstIterator InstrumentationContext::endBurst( InstrumentationContextData *icd ) const
{
   return icd->_burstList.end();
}

inline void InstrumentationContext::insertDeferredEvent ( InstrumentationContextData *icd, const Event &e )
{
   /* insert the event into the list */
   {
      LockBlock( icd->_deferredEventsLock );
      icd->_deferredEvents.push_front ( e );
   }
}

inline void InstrumentationContext::clearDeferredEvents ( InstrumentationContextData *icd )
{
   /* remove all events from the list */
   icd->_deferredEvents.clear();
}

inline size_t InstrumentationContext::getNumDeferredEvents( InstrumentationContextData *icd ) const
{
   return icd->_deferredEvents.size();
}
inline InstrumentationContextData::EventIterator InstrumentationContext::beginDeferredEvents( InstrumentationContextData *icd ) const
{
   return icd->_deferredEvents.begin();
}
inline InstrumentationContextData::EventIterator InstrumentationContext::endDeferredEvents( InstrumentationContextData *icd ) const
{
   return icd->_deferredEvents.end();
}

inline InstrumentationContextData::ConstStateIterator InstrumentationContext::beginState( InstrumentationContextData *icd ) const
{
   return icd->_stateStack.begin();
}

inline InstrumentationContextData::ConstStateIterator InstrumentationContext::endState( InstrumentationContextData *icd ) const
{
   return icd->_stateStack.end();
}

inline InstrumentationContextData::ConstStateIterator InstrumentationContext::beginSubState( InstrumentationContextData *icd ) const
{
   return icd->_subStateStack.begin();
}

inline InstrumentationContextData::ConstStateIterator InstrumentationContext::endSubState( InstrumentationContextData *icd ) const
{
   return icd->_subStateStack.end();
}

inline void InstrumentationContext::disableStateEvents ( InstrumentationContextData *icd )
{
   icd->_stateEventEnabled = false;
}

inline void InstrumentationContext::enableStateEvents ( InstrumentationContextData *icd )
{
   icd->_stateEventEnabled = true;
}

inline bool InstrumentationContext::isStateEventEnabled ( InstrumentationContextData *icd )
{
   return icd->_stateEventEnabled;
}

#endif

#endif
