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
#include "basethread.hpp"
#include "debug.hpp"
#include "system.hpp"
#include "workdescriptor.hpp"
#include "smpdd.hpp"
#include "gpudd.hpp"
#include "plugin.hpp"
#include "instrumentation.hpp"
#include "instrumentationmodule_decl.hpp"

using namespace nanos;

// TODO: move to dependent part
const size_t nanos_smp_dd_size = sizeof(ext::SMPDD);

#ifdef GPU_DEV
const size_t nanos_gpu_dd_size = sizeof(ext::GPUDD);

void * nanos_gpu_factory( void *prealloc, void *args )
{
   nanos_smp_args_t *smp = ( nanos_smp_args_t * ) args;
   if ( prealloc != NULL )
   {
      return ( void * )new (prealloc) ext::GPUDD( smp->outline );
   }
   else
   {
      return ( void * ) new ext::GPUDD( smp->outline );
   }
}
#endif

void * nanos_smp_factory( void *prealloc, void *args )
{
   nanos_smp_args_t *smp = ( nanos_smp_args_t * ) args;

   if ( prealloc != NULL )
   {
      return ( void * )new (prealloc) ext::SMPDD( smp->outline );
   }
   else 
   {
      return ( void * )new ext::SMPDD( smp->outline );
   }
}

nanos_wd_t nanos_current_wd()
{
   nanos_wd_t cwd = myThread->getCurrentWD();

   return cwd;
}

int nanos_get_wd_id ( nanos_wd_t wd )
{
   WD *lwd = ( WD * )wd;
   int id = lwd->getId();

   return id;
}

/*! \brief Creates a new WorkDescriptor
 *
 *  \sa nanos::WorkDescriptor
 */
nanos_err_t nanos_create_wd (  nanos_wd_t *uwd, size_t num_devices, nanos_device_t *devices, size_t data_size, int data_align,
                               void ** data, nanos_wg_t uwg, nanos_wd_props_t *props, size_t num_copies, nanos_copy_data_t **copies )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","*_create_wd",NANOS_CREATION) );

   try 
   {
      if ( ( props == NULL  || ( props != NULL  && !props->mandatory_creation ) ) && !sys.throttleTask() ) {
         *uwd = 0;
         return NANOS_OK;
      }
      sys.createWD ( (WD **) uwd, num_devices, devices, data_size, data_align, (void **) data, (WG *) uwg, props, num_copies, copies, NULL );

   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_set_translate_function ( nanos_wd_t wd, nanos_translate_args_t translate_args )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","*_set_translate_function",NANOS_CREATION) );
   try 
   {
      WD *lwd = ( WD * ) wd;
      lwd->setTranslateArgs( translate_args );
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

/*! \brief Creates a new Sliced WorkDescriptor
 *
 *  \sa nanos::WorkDescriptor
 */
nanos_err_t nanos_create_sliced_wd ( nanos_wd_t *uwd, size_t num_devices, nanos_device_t *devices, size_t outline_data_size, int outline_data_align,
                               void ** outline_data, nanos_wg_t uwg, nanos_slicer_t slicer, size_t slicer_data_size, int slicer_data_align,
                               nanos_slicer_data_t * slicer_data, nanos_wd_props_t *props,
                               size_t num_copies, nanos_copy_data_t **copies )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","*_create_wd",NANOS_CREATION) );

   try 
   {
      if ( ( props == NULL  || ( props != NULL  && !props->mandatory_creation ) ) && !sys.throttleTask() ) {
         *uwd = 0;
         return NANOS_OK;
      }
      if ( slicer_data == NULL ) {
         return NANOS_UNKNOWN_ERR;
      }

      sys.createSlicedWD ( (WD **) uwd, num_devices, devices, outline_data_size, outline_data_align, outline_data, (WG *) uwg,
                           (Slicer *) slicer, slicer_data_size, slicer_data_align, (SlicerData *&) *slicer_data, props,
                           num_copies, copies );

   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_submit ( nanos_wd_t uwd, size_t num_deps, nanos_dependence_t *deps, nanos_team_t team )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","submit",NANOS_SCHEDULING) );

   try {
      ensure( uwd,"NULL WD received" );

      WD * wd = ( WD * ) uwd;

      if ( team != NULL ) {
         warning( "Submitting to another team not implemented yet" );
      }

      NANOS_INSTRUMENT ( static InstrumentationDictionary *ID = sys.getInstrumentation()->getInstrumentationDictionary(); )

      NANOS_INSTRUMENT ( static nanos_event_key_t create_wd_id = ID->getEventKey("create-wd-id"); )
      NANOS_INSTRUMENT ( static nanos_event_key_t create_wd_ptr = ID->getEventKey("create-wd-ptr"); )
      NANOS_INSTRUMENT ( static nanos_event_key_t wd_num_deps = ID->getEventKey("wd-num-deps"); )
      NANOS_INSTRUMENT ( static nanos_event_key_t wd_deps_ptr = ID->getEventKey("wd-deps-ptr"); )

      NANOS_INSTRUMENT ( nanos_event_key_t Keys[4]; )
      NANOS_INSTRUMENT ( nanos_event_value_t Values[4]; )

      NANOS_INSTRUMENT ( Keys[0] = create_wd_id; )
      NANOS_INSTRUMENT ( Values[0] = (nanos_event_value_t) wd->getId(); )

      NANOS_INSTRUMENT ( Keys[1] = create_wd_ptr; )
      NANOS_INSTRUMENT ( Values[1] = (nanos_event_value_t) wd; )

      NANOS_INSTRUMENT ( Keys[2] = wd_num_deps; )
      NANOS_INSTRUMENT ( Values[2] = (nanos_event_value_t) num_deps; )

      NANOS_INSTRUMENT ( Keys[3] = wd_deps_ptr; );
      NANOS_INSTRUMENT ( Values[3] = (nanos_event_value_t) deps; )

      NANOS_INSTRUMENT( sys.getInstrumentation()->raisePointEventNkvs(4, Keys, Values); )

      NANOS_INSTRUMENT (sys.getInstrumentation()->raiseOpenPtPEventNkvs ( NANOS_WD_DOMAIN, (nanos_event_id_t) wd->getId(), 0, NULL, NULL );)

      if ( deps != NULL ) {
         sys.submitWithDependencies( *wd, num_deps, deps );
         return NANOS_OK;
      }

      sys.submit( *wd );
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}


// data must be not null
nanos_err_t nanos_create_wd_and_run ( size_t num_devices, nanos_device_t *devices, size_t data_size, int data_align, void * data,
                                      size_t num_deps, nanos_dependence_t *deps, nanos_wd_props_t *props,
                                      size_t num_copies, nanos_copy_data_t *copies, nanos_translate_args_t translate_args )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","create_wd_and_run", NANOS_CREATION) );

   try {
      if ( num_devices > 1 ) warning( "Multiple devices not yet supported. Using first one" );

      // TODO: choose device
      // pre-allocate device
      char chunk[devices[0].dd_size];
      

      WD wd( ( DD* ) devices[0].factory( chunk, devices[0].arg ), data_size, data_align, data, num_copies, copies );
      wd.setTranslateArgs( translate_args );

      int pmDataSize = sys.getPMInterface().getInternalDataSize();
      char pmData[pmDataSize];
      if ( pmDataSize > 0 ) {
        wd.setInternalData(pmData);
      }

      NANOS_INSTRUMENT ( static InstrumentationDictionary *ID = sys.getInstrumentation()->getInstrumentationDictionary(); )

      NANOS_INSTRUMENT ( static nanos_event_key_t create_wd_id = ID->getEventKey("create-wd-id"); )
      NANOS_INSTRUMENT ( static nanos_event_key_t create_wd_ptr = ID->getEventKey("create-wd-ptr"); )
      NANOS_INSTRUMENT ( static nanos_event_key_t wd_num_deps = ID->getEventKey("wd-num-deps"); )
      NANOS_INSTRUMENT ( static nanos_event_key_t wd_deps_ptr = ID->getEventKey("wd-deps-ptr"); )

      NANOS_INSTRUMENT ( nanos_event_key_t Keys[4]; )
      NANOS_INSTRUMENT ( nanos_event_value_t Values[4]; ) 

      NANOS_INSTRUMENT ( Keys[0] = create_wd_id; )
      NANOS_INSTRUMENT ( Values[0] = (nanos_event_value_t) wd.getId(); )

      NANOS_INSTRUMENT ( Keys[1] = create_wd_ptr; )
      NANOS_INSTRUMENT ( Values[1] = (nanos_event_value_t) &wd; )

      NANOS_INSTRUMENT ( Keys[2] = wd_num_deps; )
      NANOS_INSTRUMENT ( Values[2] = (nanos_event_value_t) num_deps; )

      NANOS_INSTRUMENT ( Keys[3] = wd_deps_ptr; );
      NANOS_INSTRUMENT ( Values[3] = (nanos_event_value_t) deps; )

      NANOS_INSTRUMENT( sys.getInstrumentation()->raisePointEventNkvs(4, Keys, Values); )

      NANOS_INSTRUMENT (sys.getInstrumentation()->raiseOpenPtPEventNkvs ( NANOS_WD_DOMAIN, (nanos_event_id_t) wd.getId(), 0, NULL, NULL ); )

      if ( deps != NULL ) {
         sys.waitOn( num_deps, deps );
      }
      
      NANOS_INSTRUMENT( InstrumentState inst1(NANOS_RUNTIME) );
      sys.inlineWork( wd );
      NANOS_INSTRUMENT( inst1.close() );

   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_set_internal_wd_data ( nanos_wd_t wd, void *data )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","set_internal_wd_data",NANOS_RUNTIME) );

   try {
      WD *lwd = ( WD * ) wd;

      lwd->setInternalData( data );
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_get_internal_wd_data ( nanos_wd_t wd, void **data )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","get_internal_wd_data",NANOS_RUNTIME) );

   try {
      WD *lwd = ( WD * ) wd;
      void *ldata;

      ldata = lwd->getInternalData();

      *data = ldata;
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

nanos_err_t nanos_yield ( void )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","yield",NANOS_SCHEDULING) );

   try {
      Scheduler::yield();

   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}


nanos_err_t nanos_slicer_get_specific_data ( nanos_slicer_t slicer, void ** data )
{                                                                                                                                                        
   //NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","get_specific_data",NANOS_RUNTIME) );

   try {
      *data = ((Slicer *)slicer)->getSpecificData();
   } catch ( ... ) {                                                                                                                                     
      return NANOS_UNKNOWN_ERR;                                                                                                                          
   }                                                                                                                                                     
                                                                                                                                                         
   return NANOS_OK;                                                                                                                                      
}   

