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

#include "smpprocessor.hpp"
#include "schedule.hpp"
#include "debug.hpp"
#include "system.hpp"
#include "smp_ult.hpp"
#include "instrumentation.hpp"

using namespace nanos;
using namespace nanos::ext;

SMPDevice nanos::ext::SMP( "SMP" );

size_t SMPDD::_stackSize = 32*1024;

/*!
  \brief Registers the Device's configuration options
  \param reference to a configuration object.
  \sa Config System
*/
void SMPDD::prepareConfig( Config &config )
{
   /*!
      Get the stack size from system configuration
    */
   size_t size = sys.getDeviceStackSize(); 
   if ( size > 0 )
      _stackSize = size;

   /*!
      Get the stack size for this device
   */
   config.registerConfigOption ( "smp-stack-size", NEW Config::SizeVar( _stackSize ), "Defines SMP workdescriptor stack size" );
   config.registerArgOption ( "smp-stack-size", "smp-stack-size" );
   config.registerEnvOption ( "smp-stack-size", "NX_SMP_STACK_SIZE" );
}

void SMPDD::initStack ( void *data )
{
#ifdef NANOS_INSTRUMENTATION_ENABLED
   _state = ::initContext( _stack, _stackSize, ( void * )&workWrapper,data,( void * )Scheduler::exit, 0 );
#else
   _state = ::initContext( _stack, _stackSize, ( void * )getWorkFct(),data,( void * )Scheduler::exit, 0 );
#endif
}

void SMPDD::workWrapper( void *data )
{
   SMPDD &dd = ( SMPDD & ) myThread->getCurrentWD()->getActiveDevice();

   NANOS_INSTRUMENT ( static nanos_event_key_t key = sys.getInstrumentation()->getInstrumentationDictionary()->getEventKey("user-code") );
   NANOS_INSTRUMENT ( nanos_event_value_t val = myThread->getCurrentWD()->getId() );
   NANOS_INSTRUMENT ( sys.getInstrumentation()->raiseOpenStateAndBurst ( NANOS_RUNNING, key, val ) );
   dd.getWorkFct()( data );
   NANOS_INSTRUMENT ( sys.getInstrumentation()->raiseCloseStateAndBurst ( key ) );
}

void SMPDD::lazyInit (WD &wd, bool isUserLevelThread, WD *previous)
{
   if (isUserLevelThread) {
     if ( previous == NULL )
       _stack = NEW intptr_t[_stackSize];
     else {
        SMPDD &oldDD = (SMPDD &) previous->getActiveDevice();

        std::swap(_stack,oldDD._stack);
     }
  
     initStack(wd.getData());
   }
}

SMPDD * SMPDD::copyTo ( void *toAddr )
{
   SMPDD *dd = new (toAddr) SMPDD(*this);
   return dd;
}

