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

#include "os.hpp"
#include "plugin.hpp"
#include "config.hpp"
#include "config.h"
#include "system.hpp"
#include <string>
#include <stdio.h>
#include <iostream>
#include <dirent.h>
#include <stdlib.h>
#include "compatibility.hpp"

using namespace nanos;

void utilInit ( void * ) 
{
   sys.setDelayedStart(true);
}

#define INIT_NULL { utilInit, 0 }                                                                                                                                         
LINKER_SECTION(nanos_init, nanos_init_desc_t , INIT_NULL) 

int main (int argc, char* argv[])
{

   struct dirent **namelist;
   int n;

   bool listHelp = false;
   bool listPlugins = false;
   std::string arg;

   for ( int i=1; i < argc; i++ ) {
      arg = std::string( argv[i] );

      if ( arg.compare( "--help" ) == 0 ) {
         listHelp = true;
      } else if ( arg.compare( "--list-modules" ) == 0 ) {
         listPlugins = true;
      } else {
         std::cout << "usage: " << argv[0] << " [--help] [--list-modules]" << std::endl;
         exit(0);
      }
   } 

   if ( !listPlugins && !listHelp ) {
      std::cout << "usage: " << argv[0] << " [--help] [--list-modules]" << std::endl;
      exit(0);
   }

   if ( listPlugins )
      std::cout << "Nanox runtime library available plugins at '" << PLUGIN_DIR << "':" << std::endl;

   n = scandir( PLUGIN_DIR, &namelist, 0, alphasort );

   if ( n < 0 )
      perror( "scandir" );
   else {
      while ( n-- ) {
         std::string name( namelist[n]->d_name );

         if ( name.compare( 0,9,"libnanox-" ) != 0 ) continue;

         if ( name.compare( name.size()-3,3,".so" ) == 0 ) {
            name.erase( name.size()-3 );
            name.erase( 0, std::string("libnanox-").size() );

            if ( PluginManager::isPlugin( name ) ) {
               Plugin *plugin = PluginManager::loadAndGetPlugin( name , false );

               if ( listPlugins && plugin != NULL )
                  std::cout << "   " << name << " - " << plugin->getName() << " - version " << plugin->getVersion() << std::endl;
            }
         }

         free( namelist[n] );
      }

      free( namelist );
   }

   if ( listHelp ) {
      if ( listPlugins )
         std::cout << std::endl;

      std::cout << "Nanos++ runtime library version " << VERSION << "." << std::endl;
      std::cout << std::endl;
      std::cout << "The runtime configuration can be set using arguments added" << std::endl;
      std::cout << "to the NX_ARGS environment variable or through specialised" << std::endl;
      std::cout << "environment variables. As an example of NX_ARGS, to execute" << std::endl;
      std::cout << "with verbose mode and 4 PEs the NX_ARGS environment variable" << std::endl;
      std::cout << "should be: 'NX_ARGS=\"--pes=4 --verbose\"'." << std::endl;
      std::cout << std::endl;
      std::cout << "All NX_ARGS and env vars are listed below." << std::endl;
      std::cout << std::endl;

      std::cout << Config::getNanosHelp();
   }

}
