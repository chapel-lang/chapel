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
#include "nanos.h"
#include "schedule.hpp"
#include "system.hpp"
#include "synchronizedcondition.hpp"
#include "instrumentationmodule_decl.hpp"
#include "instrumentation.hpp"

using namespace nanos;

nanos_err_t nanos_wg_wait_completion ( nanos_wg_t uwg )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","wg_wait_completion",NANOS_SYNCHRONIZATION) );

   try {
      WG *wg = ( WG * )uwg;
      wg->waitCompletion();
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_create_int_sync_cond ( nanos_sync_cond_t *sync_cond, volatile int *p, int condition )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","*_create_sync_cond",NANOS_RUNTIME ) );

   try {
      *sync_cond = ( nanos_sync_cond_t * ) new SingleSyncCond<EqualConditionChecker<int> >( EqualConditionChecker<int>( p, condition ) );
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_create_bool_sync_cond ( nanos_sync_cond_t *sync_cond, volatile bool *p, bool condition )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","*_create_sync_cond",NANOS_RUNTIME) );

   try {
      *sync_cond = ( nanos_sync_cond_t * ) new SingleSyncCond<EqualConditionChecker<bool> >( EqualConditionChecker<bool>( p, condition ) );
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_sync_cond_wait ( nanos_sync_cond_t *sync_cond )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","sync_cond_wait",NANOS_SYNCHRONIZATION) );

   try {
      GenericSyncCond * syncCond = (GenericSyncCond *) *sync_cond;
      syncCond->wait();
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_sync_cond_signal ( nanos_sync_cond_t *sync_cond )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","sync_cond_signal",NANOS_SYNCHRONIZATION) );

   try {
      GenericSyncCond * syncCond = (GenericSyncCond *) *sync_cond;
      syncCond->signal();
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_destroy_sync_cond ( nanos_sync_cond_t *sync_cond )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","destroy_sync_cond",NANOS_RUNTIME) );

   try {
      GenericSyncCond * syncCond = (GenericSyncCond *) *sync_cond;
      delete syncCond;
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_wait_on ( size_t num_deps, nanos_dependence_t *deps )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","wait_on",NANOS_SYNCHRONIZATION ); )

   NANOS_INSTRUMENT ( static InstrumentationDictionary *ID = sys.getInstrumentation()->getInstrumentationDictionary(); )

   NANOS_INSTRUMENT ( static nanos_event_key_t wd_num_deps = ID->getEventKey("wd-num-deps"); )
   NANOS_INSTRUMENT ( static nanos_event_key_t wd_deps_ptr = ID->getEventKey("wd-deps-ptr"); )
                                                                                                                                                         
   NANOS_INSTRUMENT ( nanos_event_key_t Keys[2]; )
   NANOS_INSTRUMENT ( nanos_event_value_t Values[2]; )

   NANOS_INSTRUMENT ( Keys[0] = wd_num_deps; )
   NANOS_INSTRUMENT ( Values[0] = (nanos_event_value_t) num_deps; )

   NANOS_INSTRUMENT ( Keys[1] = wd_deps_ptr; );
   NANOS_INSTRUMENT ( Values[1] = (nanos_event_value_t) deps; )

   NANOS_INSTRUMENT( sys.getInstrumentation()->raisePointEventNkvs(2, Keys, Values); )

   try {
      if ( deps != NULL ) {
         sys.waitOn( num_deps, deps );
         return NANOS_OK;
      }

   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_init_lock ( nanos_lock_t **lock )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","init_lock",NANOS_RUNTIME) );

   NANOS_INSTRUMENT ( static InstrumentationDictionary *ID = sys.getInstrumentation()->getInstrumentationDictionary(); )

   NANOS_INSTRUMENT ( static nanos_event_key_t Keys = ID->getEventKey("lock-addr"); )
   NANOS_INSTRUMENT ( nanos_event_value_t Values = (nanos_event_value_t) *lock; )
   NANOS_INSTRUMENT( sys.getInstrumentation()->raisePointEventNkvs(1, &Keys, &Values); )

   try {
      *lock = new Lock();
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_set_lock ( nanos_lock_t *lock )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","set_lock",NANOS_SYNCHRONIZATION) );

   NANOS_INSTRUMENT ( static InstrumentationDictionary *ID = sys.getInstrumentation()->getInstrumentationDictionary(); )

   NANOS_INSTRUMENT ( static nanos_event_key_t Keys = ID->getEventKey("lock-addr"); )
   NANOS_INSTRUMENT ( nanos_event_value_t Values = (nanos_event_value_t) lock; )
   NANOS_INSTRUMENT( sys.getInstrumentation()->raisePointEventNkvs(1, &Keys, &Values); )

   try {
      Lock &l = *( Lock * ) lock;
      l++;
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_unset_lock ( nanos_lock_t *lock )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","unset_lock",NANOS_SYNCHRONIZATION) );

   NANOS_INSTRUMENT ( static InstrumentationDictionary *ID = sys.getInstrumentation()->getInstrumentationDictionary(); )

   NANOS_INSTRUMENT ( static nanos_event_key_t Keys = ID->getEventKey("lock-addr"); )
   NANOS_INSTRUMENT ( nanos_event_value_t Values = (nanos_event_value_t) lock; )
   NANOS_INSTRUMENT( sys.getInstrumentation()->raisePointEventNkvs(1, &Keys, &Values); )

   try {
      Lock &l = *( Lock * ) lock;
      l--;
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_try_lock ( nanos_lock_t *lock, bool *result )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","try_lock",NANOS_SYNCHRONIZATION) );

   NANOS_INSTRUMENT ( static InstrumentationDictionary *ID = sys.getInstrumentation()->getInstrumentationDictionary(); )

   NANOS_INSTRUMENT ( static nanos_event_key_t Keys = ID->getEventKey("lock-addr"); )
   NANOS_INSTRUMENT ( nanos_event_value_t Values = (nanos_event_value_t) lock; )
   NANOS_INSTRUMENT( sys.getInstrumentation()->raisePointEventNkvs(1, &Keys, &Values); )

   try {
      Lock &l = *( Lock * ) lock;

      *result = l.tryAcquire();
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_destroy_lock ( nanos_lock_t *lock )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","destroy_lock",NANOS_RUNTIME) );

   NANOS_INSTRUMENT ( static InstrumentationDictionary *ID = sys.getInstrumentation()->getInstrumentationDictionary(); )

   NANOS_INSTRUMENT ( static nanos_event_key_t Keys = ID->getEventKey("lock-addr"); )
   NANOS_INSTRUMENT ( nanos_event_value_t Values = (nanos_event_value_t) lock; )
   NANOS_INSTRUMENT( sys.getInstrumentation()->raisePointEventNkvs(1, &Keys, &Values); )

   try {
      delete ( Lock * )lock;
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}


nanos_err_t nanos_single_guard ( bool *b )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","single_guard",NANOS_SYNCHRONIZATION) );

   try {
      *b = myThread->singleGuard();
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

