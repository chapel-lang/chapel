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
#ifndef __NANOS_INSTRUMENTOR_MODULE_DECL_H
#define __NANOS_INSTRUMENTOR_MODULE_DECL_H
#include "debug.hpp"
#include "nanos-int.h"
#include "instrumentation.hpp"
#include "system.hpp"

namespace nanos {

#ifdef NANOS_INSTRUMENTATION_ENABLED

   class InstrumentStateAndBurst {
      private:
         Instrumentation     &_inst;   /**< Instrumentation object*/
         nanos_event_key_t    _key;    /**< Key used in burst event */
	 bool		      _closed; /**< Closed flag */
      private:
         /*! \brief InstrumentStateAndBurst default constructor (private)
          */
         InstrumentStateAndBurst ();
         /*! \brief InstrumentStateAndBurst copy constructor (private)
          */
         InstrumentStateAndBurst ( InstrumentStateAndBurst &isb );
         /*! \brief InstrumentStateAndBurst copy assignment operator (private)
          */
         InstrumentStateAndBurst& operator= ( InstrumentStateAndBurst &isb );
      public:
         /*! \brief InstrumentStateAndBurst constructor
          */
         InstrumentStateAndBurst ( const char* keydesc, const char *valdesc, nanos_event_state_value_t state )
            : _inst(*sys.getInstrumentation()), _key( _inst.getInstrumentationDictionary()->getEventKey(keydesc)),
              _closed(false)
         {
            nanos_event_value_t val = _inst.getInstrumentationDictionary()->getEventValue(keydesc,valdesc);
            _inst.raiseOpenStateAndBurst(state, _key, val);
         }
         /*! \brief InstrumentStateAndBurst constructor
          */
         InstrumentStateAndBurst ( const char* keydesc, nanos_event_value_t val, nanos_event_state_value_t state )
            : _inst(*sys.getInstrumentation()), _key( _inst.getInstrumentationDictionary()->getEventKey(keydesc)),
              _closed(false)
         {
            _inst.raiseOpenStateAndBurst(state, _key, val);
         }
         /*! \brief InstrumentStateAndBurst destructor
          */
         ~InstrumentStateAndBurst ( ) { if (!_closed) close(); }
         /*! \brief Closes states and burst
          */
	 void close() { _closed=true; _inst.raiseCloseStateAndBurst(_key);  }
   };

   class InstrumentState {
      private:
         Instrumentation     &_inst;    /**< Instrumentation object*/
	 bool		      _closed;  /**< Closed flag */
      private:
         /*! \brief InstrumentState default constructor (private)
          */
         InstrumentState ();
         /*! \brief InstrumentState copy constructor (private)
          */
         InstrumentState ( InstrumentState &is );
         /*! \brief InstrumentState copy assignment operator (private)
          */
         InstrumentState& operator= ( InstrumentState &is );
      public:
         /*! \brief InstrumentState constructor
          */
         InstrumentState ( nanos_event_state_value_t state )
            : _inst(*sys.getInstrumentation()), _closed(false)
         {
            _inst.raiseOpenStateEvent( state );
         }
         /*! \brief InstrumentState destructor 
          */
         ~InstrumentState ( ) { if (!_closed) close(); }
         /*! \brief Closes states
          */
	 void close() { _closed=true; _inst.raiseCloseStateEvent();  }
   };

   class InstrumentSubState {
      private:
         Instrumentation     &_inst; /**< Instrumentation object */
      private:
         /*! \brief InstrumentSubState default constructor (private)
          */
         InstrumentSubState ();
         /*! \brief InstrumentSubState copy constructor (private)
          */
         InstrumentSubState ( InstrumentSubState &iss );
         /*! \brief InstrumentSubState copy assignment operator (private)
          */
         InstrumentSubState& operator= ( InstrumentSubState &iss );
      public:
         /*! \brief InstrumentSubState constructor
          */
         InstrumentSubState ( nanos_event_state_value_t subState )
            : _inst(*sys.getInstrumentation())
         {
            _inst.disableStateEvents(subState);
         }
         /*! \brief InstrumentSubState destructor
          */
         ~InstrumentSubState ()
         {
            _inst.enableStateEvents();
         }
   };

#endif
}
#endif
