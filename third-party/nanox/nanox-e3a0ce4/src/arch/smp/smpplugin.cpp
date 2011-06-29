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

#include "plugin.hpp"
#include "smpprocessor.hpp"
#include "smpdd.hpp"
#include "system.hpp"

namespace nanos {
namespace ext {

PE * smpProcessorFactory ( int id )
{
   return new SMPProcessor( id );
}

class SMPPlugin : public Plugin
{

   public:
      SMPPlugin() : Plugin( "SMP PE Plugin",1 ) {}

      virtual void config( Config& config )
      {
         config.setOptionsSection( "SMP Arch", "SMP specific options" );
         SMPProcessor::prepareConfig( config );
         SMPDD::prepareConfig( config );
      }

      virtual void init() {
         sys.setHostFactory( smpProcessorFactory );
      }
};
}
}

nanos::ext::SMPPlugin NanosXPlugin;

