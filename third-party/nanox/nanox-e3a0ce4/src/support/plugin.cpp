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

#include "debug.hpp"
#include "plugin.hpp"
#include "os.hpp"
#include "config.hpp"

using namespace nanos;

PluginManager::PluginList PluginManager::_activePlugins;

void PluginManager::init()
{
   Config config;
   config.init();
}

bool PluginManager::isPlugin ( const char *name )
{
   std::string dlname;
   void * handler;

   dlname = "libnanox-";
   dlname += name;
   handler = OS::loadDL( "",dlname );

   if ( !handler ) {
      warning0 ( "plugin error=" << OS::dlError( handler ) );
      return false;
   }

   Plugin *plugin = ( Plugin * ) OS::dlFindSymbol( handler, "NanosXPlugin" );

   return plugin != NULL;
}

bool PluginManager::load ( const char *name, const bool init )
{
   return loadAndGetPlugin ( name, init ) != NULL;
}

Plugin * PluginManager::loadAndGetPlugin( const char *name, const bool init )
{
   std::string dlname;
   void * handler;

   dlname = "libnanox-";
   dlname += name;
   handler = OS::loadDL( "",dlname );

   if ( !handler ) {
      warning0 ( "plugin error=" << OS::dlError( handler ) );
      return NULL;
   }

   Plugin *plugin = ( Plugin * ) OS::dlFindSymbol( handler, "NanosXPlugin" );

   if ( !plugin ) {
      warning0 ( "plugin error=" << OS::dlError( handler ) );
      return NULL;
   }

 
   Config config;
   plugin->config(config);
   config.init();
   
   if ( init )
      plugin->init();

   return plugin;
}
