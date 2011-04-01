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

#include "nanos.h"
#include "system.hpp"
#include "instrumentation.hpp"
#include <alloca.h>

#ifdef GPU_DEV
#include "gputhread.hpp"
#endif

using namespace nanos;

nanos_err_t nanos_instrument_register_key ( nanos_event_key_t *event_key, const char *key, const char *description, bool abort_when_registered )
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   try
   {
      *event_key = sys.getInstrumentation()->getInstrumentationDictionary()->registerEventKey(key, description, abort_when_registered);
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
   return NANOS_OK;
}

nanos_err_t nanos_instrument_register_value ( nanos_event_value_t *event_value, const char *key, const char *value, const char *description, bool abort_when_registered )
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   try
   {
      *event_value = sys.getInstrumentation()->getInstrumentationDictionary()->registerEventValue(key, value,  description, abort_when_registered);
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
   return NANOS_OK;
}

nanos_err_t nanos_instrument_register_value_with_val ( nanos_event_value_t val, const char *key, const char *value, const char *description, bool abort_when_registered )
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   try
   {
      sys.getInstrumentation()->getInstrumentationDictionary()->registerEventValue(key, value, val, description, abort_when_registered);
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
   return NANOS_OK;
}

nanos_err_t nanos_instrument_get_key (const char *key, nanos_event_key_t *event_key)
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   try
   {
      *event_key = sys.getInstrumentation()->getInstrumentationDictionary()->getEventKey(key);
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
   return NANOS_OK;
}

nanos_err_t nanos_instrument_get_value (const char *key, const char *value, nanos_event_value_t *event_value)
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   try
   {
      *event_value = sys.getInstrumentation()->getInstrumentationDictionary()->getEventValue(key, value);
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
   return NANOS_OK;
}

nanos_err_t nanos_instrument_events ( unsigned int num_events, nanos_event_t events[] )
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   try
   {
      Instrumentation::Event *e = (Instrumentation::Event *) alloca ( sizeof(Instrumentation::Event) * num_events ); 

      for (unsigned int i = 0; i < num_events; i++ ) {
         switch ( events[i].type ) {
            case NANOS_STATE_START:
               sys.getInstrumentation()->createStateEvent(&e[i],events[i].info.state.value);
               break;
            case NANOS_STATE_END:
               sys.getInstrumentation()->returnPreviousStateEvent(&e[i]);
               break;
            case NANOS_BURST_START:
               sys.getInstrumentation()->createBurstEvent(&e[i],events[i].info.burst.key,events[i].info.burst.value);
               break;
            case NANOS_BURST_END:
               sys.getInstrumentation()->closeBurstEvent(&e[i],events[i].info.burst.key);
               break;
            case NANOS_POINT:
               sys.getInstrumentation()->createPointEvent(&e[i],events[i].info.point.nkvs,events[i].info.point.keys,events[i].info.point.values );
               break;
            case NANOS_PTP_START:
               sys.getInstrumentation()->createPtPStart(&e[i],events[i].info.ptp.domain,events[i].info.ptp.id,events[i].info.ptp.nkvs,events[i].info.ptp.keys,events[i].info.ptp.values);
               break;
            case NANOS_PTP_END:
               sys.getInstrumentation()->createPtPEnd(&e[i],events[i].info.ptp.domain,events[i].info.ptp.id,events[i].info.ptp.nkvs,events[i].info.ptp.keys,events[i].info.ptp.values);
               break;
            default:
               return NANOS_UNKNOWN_ERR;
         }
      }

      sys.getInstrumentation()->addEventList( num_events,e);
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
   return NANOS_OK;
}

nanos_err_t nanos_instrument_enter_state ( nanos_event_state_value_t state )
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   try
   {
      sys.getInstrumentation()->raiseOpenStateEvent( state );
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
   return NANOS_OK;
}

nanos_err_t nanos_instrument_leave_state ( void )
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   try
   {
      sys.getInstrumentation()->raiseCloseStateEvent( );
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
   return NANOS_OK;
}

nanos_err_t nanos_instrument_enter_burst( nanos_event_key_t key, nanos_event_value_t value )
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   try
   {
      sys.getInstrumentation()->raiseOpenBurstEvent ( key, value );
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
   return NANOS_OK;
}

nanos_err_t nanos_instrument_leave_burst( nanos_event_key_t key )
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   try
   {
      sys.getInstrumentation()->raiseCloseBurstEvent ( key );
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
   return NANOS_OK;
}

nanos_err_t nanos_instrument_point_event ( unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values )
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   try
   {
      sys.getInstrumentation()->raisePointEventNkvs ( nkvs, keys, values );
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
   return NANOS_OK;
}

nanos_err_t nanos_instrument_ptp_start ( nanos_event_domain_t domain, nanos_event_id_t id,
                                         unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values )
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   try
   {
      sys.getInstrumentation()->raiseOpenPtPEventNkvs ( domain, id, nkvs, keys, values );
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
   return NANOS_OK;
}

nanos_err_t nanos_instrument_ptp_end ( nanos_event_domain_t domain, nanos_event_id_t id,
                                         unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values )
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   try
   {
      sys.getInstrumentation()->raiseClosePtPEventNkvs ( domain, id, nkvs, keys, values );
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
   return NANOS_OK;
}

nanos_err_t nanos_instrument_disable_state_events ( nanos_event_state_value_t state )
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   try
   {
      sys.getInstrumentation()->disableStateEvents( state );
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
   return NANOS_OK;
}

nanos_err_t nanos_instrument_enable_state_events ( void )
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   try
   {
      sys.getInstrumentation()->enableStateEvents();
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
   return NANOS_OK;
}


nanos_err_t nanos_instrument_close_user_fun_event()
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
#ifdef GPU_DEV
   try
   {
      ( ( ext::GPUThread *) myThread )->enableWDClosingEvents();
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
#endif
#endif
   return NANOS_OK;
}
