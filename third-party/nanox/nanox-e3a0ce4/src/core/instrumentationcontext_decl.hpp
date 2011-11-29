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
#ifndef __NANOS_INSTRUMENTOR_CTX_DECL_H
#define __NANOS_INSTRUMENTOR_CTX_DECL_H
#include <stack>
#include <list>

#include "instrumentation_decl.hpp"
#include "debug.hpp"

namespace nanos {

   class InstrumentationContextData {
         friend class InstrumentationContext;
         friend class InstrumentationContextStackedStates;
         friend class InstrumentationContextStackedBursts;
         friend class InstrumentationContextStackedStatesAndBursts;
      public:
         typedef Instrumentation::Event                  Event;                /**< Class defined in instrumentation_decl.hpp */
         typedef Instrumentation::Burst                  Burst;                /**< Class defined in instrumentation_decl.hpp */
         typedef std::deque<nanos_event_state_value_t>   StateStack;           /**< Stack of state's values */
         typedef std::list<Event>                        EventList;            /**< List of Events (Bursts) */
         typedef EventList::const_iterator               ConstBurstIterator;   /**< InstrumentationContext const BurstIterator */
         typedef EventList::iterator                     BurstIterator;        /**< InstrumentationContext BurstIterator */
         typedef StateStack::const_iterator              ConstStateIterator;   /**< InstrumentationContext const StateIterator*/
         typedef StateStack::iterator                    StateIterator;        /**< InstrumentationContext StateIterator */
         typedef EventList::const_iterator               ConstEventIterator;   /**< InstrumentationContext const EventIterator */
         typedef EventList::iterator                     EventIterator;        /**< InstrumentationContext EventIterator */
#ifdef NANOS_INSTRUMENTATION_ENABLED
      private: /* Only friend classes (InstrumentationContext...) can use InstrumentationContextData */
         bool                       _startingWD;             /**< Is a startingWD? */
         StateStack                 _stateStack;             /**< Stack of states */
         StateStack                 _subStateStack;          /**< Stack of sub states */
         bool                       _stateEventEnabled;      /**< Set state level, zero by default */
         EventList                  _burstList;              /**< List of current opened bursts */
         EventList                  _burstBackup;            /**< Backup list (non-active) of opened bursts */
         EventList                  _deferredEvents;         /**< List of deferred events */
         Lock                       _deferredEventsLock;     /**< Lock in deferred event list */
      private:
         /*! \brief InstrumentationContextData copy assignment operator (private)
          */
         InstrumentationContextData& operator= (const InstrumentationContextData &icd); 
      public:
         /*! \brief InstrumentationContextData copy constructor
          */
         explicit InstrumentationContextData(const InstrumentationContextData &icd) : _startingWD(false), _stateStack(), _subStateStack(), 
                  _stateEventEnabled(icd._stateEventEnabled), _burstList(), _burstBackup(), _deferredEvents(), _deferredEventsLock() {}
         /*! \brief InstrumentationContextData copy constructor
          */
         explicit InstrumentationContextData(const InstrumentationContextData *icd) : _startingWD(false), _stateStack(), _subStateStack(),
                  _stateEventEnabled(icd->_stateEventEnabled), _burstList(), _burstBackup(), _deferredEvents(), _deferredEventsLock() {}
         /*! \brief InstrumentationContextData default constructor
          */
         InstrumentationContextData() : _startingWD(false), _stateStack(), _subStateStack(),
                   _stateEventEnabled(true), _burstList(), _burstBackup(), _deferredEvents(), _deferredEventsLock() { }
         /*! \brief InstrumentationContextData destructor
          */
         ~InstrumentationContextData() {}
         /*! \brief Sets _startingWD attribute
          */
         void setStartingWD ( bool value ) { _startingWD = value; }
         /*! \brief Sets _startingWD attribute
          */
         bool getStartingWD ( void ) { return _startingWD; }
#else
      private:
         /*! \brief InstrumentationContextData copy constructor (private)
          */
         InstrumentationContextData(const InstrumentationContextData &icd); 
         /*! \brief InstrumentationContextData copy assignment operator (private)
          */
         InstrumentationContextData& operator= (const InstrumentationContextData &icd); 
      public:
         /*! \brief InstrumentationContextData constructor (empty version)
          */
         InstrumentationContextData() {}
         /*! \brief InstrumentationContextData destructor (empty version)
          */
         ~InstrumentationContextData() {}
#endif
   };

#ifdef NANOS_INSTRUMENTATION_ENABLED

   class InstrumentationContext {
         friend class Instrumentation;
      public:
         typedef Instrumentation::Event                  Event;                /**< Class defined in instrumentation_decl.hpp */
      private:
         /*! \brief InstrumentationContext copy constructor (private)
          */
         InstrumentationContext ( const InstrumentationContext &ic );
         /*! \brief InstrumentationContext copy assignment operator (private)
          */
         InstrumentationContext& operator=  ( const InstrumentationContext &ic );
      public:
         /*! \brief InstrumentationContext default constructor
          */
         InstrumentationContext () {}
         /*! \brief InstrumentationContext destructor
          */
         virtual ~InstrumentationContext() {}
         /*! \brief Adds a state value into the state stack 
          */
         void pushState ( InstrumentationContextData *icd, nanos_event_state_value_t state ); 
         /*! \brief Removes top state from the state stack
          */
         void popState ( InstrumentationContextData *icd ); 
         /*! \brief Gets current state/substate from top of stack (acording with current behaviour)
          */
         nanos_event_state_value_t topState ( InstrumentationContextData *icd );
         /*! \brief Gets current state from top of stack
          */
         nanos_event_state_value_t getState ( InstrumentationContextData *icd );
         /*! \brief Gets current substate from top of stack
          */
         nanos_event_state_value_t getSubState ( InstrumentationContextData *icd );
         /*! \brief Gets stack of state's size
          */
         size_t getStateStackSize ( InstrumentationContextData *icd );
         /*! \brief Gets stack of substate's size 
          */
         size_t getSubStateStackSize ( InstrumentationContextData *icd );

         /*! \brief Inserts a Burst into the burst list
          *
          *  This function inserts a burst event in the burst list. If an event with the same type of that
          *  event was already in the list it will be moved to an internal backup list in order to guarantee
          *  just one event per type in the list.
          */
         virtual void insertBurst ( InstrumentationContextData *icd, const Event &e );
         /*! \brief Removes a Burst from the burst list
          *
          *  This function removes a burst event from the burst list. If an event with the same type of that
          *  event was in the backup list it will be recovered to main list.
          */
         virtual void removeBurst ( InstrumentationContextData *icd, InstrumentationContextData::BurstIterator it ); 
         /*! \brief Look for a specific event given its key value
          */
         bool findBurstByKey ( InstrumentationContextData *icd, nanos_event_key_t key, InstrumentationContextData::BurstIterator &ret );
         /*! \brief Get the number of bursts in the main list
          */
         size_t getNumBursts( InstrumentationContextData *icd ) const ; 
         /*! \brief Gets the starting element in the burst list
          */
         InstrumentationContextData::ConstBurstIterator beginBurst( InstrumentationContextData *icd ) const ; 
         /*! \brief Gets the last element in the burst list
          */
         InstrumentationContextData::ConstBurstIterator endBurst( InstrumentationContextData *icd ) const ; 
         /*! \brief Inserts a deferred event into the deferred event list
          *
          *  This function inserts a deferred event in the deferred event list. 
          */
         void insertDeferredEvent ( InstrumentationContextData *icd, const Event &e );
         /*! \brief Removes all deferred events from deferred event list
          */
         void clearDeferredEvents ( InstrumentationContextData *icd ); 
         /*! \brief Gets the size of deferred event list
          */
         inline size_t getNumDeferredEvents( InstrumentationContextData *icd ) const ;
         /*! \brief Gets the starting element in the deferred event list
          */
         InstrumentationContextData::EventIterator beginDeferredEvents( InstrumentationContextData *icd ) const ;
         /*! \brief Gets the last element in the deferred event list 
          */
         InstrumentationContextData::EventIterator endDeferredEvents( InstrumentationContextData *icd ) const ;

         /*! \brief Gets the starting element in the state stack
          */
         InstrumentationContextData::ConstStateIterator beginState( InstrumentationContextData *icd ) const ; 
         /*! \brief Gets the last element in the state stack
          */
         InstrumentationContextData::ConstStateIterator endState( InstrumentationContextData *icd ) const ; 
         /*! \brief Gets the starting element in the sub-state stack
          */
         InstrumentationContextData::ConstStateIterator beginSubState( InstrumentationContextData *icd ) const ; 
         /*! \brief Gets the last element in the sub-state stack
          */
         InstrumentationContextData::ConstStateIterator endSubState( InstrumentationContextData *icd ) const ; 
         /*! \brief Enable state events
          */
         void enableStateEvents ( InstrumentationContextData *icd ) ;
         /*! \brief Disable state events
          */
         void disableStateEvents ( InstrumentationContextData *icd ) ;
         /*! \brief Get state events status
          */
         bool isStateEventEnabled ( InstrumentationContextData *icd ) ;
         /*!
          */
         virtual bool showStackedBursts( void );
         /*!
          */
         virtual bool showStackedStates( void );
   };

   class InstrumentationContextStackedStates : public InstrumentationContext {
      private:
         InstrumentationContextStackedStates ( const InstrumentationContextStackedStates &icss );
         InstrumentationContextStackedStates& operator= ( const InstrumentationContextStackedStates &icss );
      public:
         InstrumentationContextStackedStates () : InstrumentationContext() {}
         ~InstrumentationContextStackedStates () {}
    
         bool showStackedBursts( void );
         bool showStackedStates( void );
   };

   class InstrumentationContextStackedBursts : public InstrumentationContext {
      private:
         InstrumentationContextStackedBursts ( const InstrumentationContextStackedBursts &icsb );
         InstrumentationContextStackedBursts& operator= ( const InstrumentationContextStackedBursts &icsb );
      public:
         InstrumentationContextStackedBursts () : InstrumentationContext() {}
         ~InstrumentationContextStackedBursts () {}
    
         bool showStackedBursts( void );
         bool showStackedStates( void );

         void insertBurst ( InstrumentationContextData *icd, const Event &e );
         void removeBurst ( InstrumentationContextData *icd, InstrumentationContextData::BurstIterator it ); 
   };

   class InstrumentationContextStackedStatesAndBursts : public InstrumentationContext {
      private:
         InstrumentationContextStackedStatesAndBursts ( const InstrumentationContextStackedStatesAndBursts &icssb );
         InstrumentationContextStackedStatesAndBursts& operator= ( const InstrumentationContextStackedStatesAndBursts &icssb );
      public:
         InstrumentationContextStackedStatesAndBursts () : InstrumentationContext() {}
         ~InstrumentationContextStackedStatesAndBursts () {}
    
         bool showStackedBursts( void );
         bool showStackedStates( void );

         void insertBurst ( InstrumentationContextData *icd, const Event &e );
         void removeBurst ( InstrumentationContextData *icd, InstrumentationContextData::BurstIterator it ); 
   };

#endif

}
#endif
