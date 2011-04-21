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
#include "debug.hpp"
#include "system.hpp"
#include "plugin.hpp"
#include "instrumentationmodule_decl.hpp"

using namespace nanos;

/*! \brief Find a slicer giving a label id
 *
 *  \sa Slicers
 */
nanos_slicer_t nanos_find_slicer ( const char * label )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","find_slicer",NANOS_RUNTIME) );

   nanos_slicer_t slicer;
   try {
      slicer = sys.getSlicer ( std::string(label) );
      if ( slicer == NULL ) {
         std::string plugin = "slicer-" + std::string(label);
         if ( !PluginManager::load( plugin )) fatal0( "Could not load " + std::string(label) + "slicer" );
         slicer = sys.getSlicer ( std::string(label) );
      }

   } catch ( ... ) {
      return ( nanos_slicer_t ) NULL;
   }
   return slicer;
}

