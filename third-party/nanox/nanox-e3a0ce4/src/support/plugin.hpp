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

#ifndef _NANOS_PLUGIN
#define _NANOS_PLUGIN

#include <string>
#include <vector>
#include "config.hpp"
#include "plugin_decl.hpp"

using namespace nanos;

inline const std::string & Plugin::getName() const
{
   return _name;
}

inline int Plugin::getVersion() const
{
   return _version;
}

inline bool PluginManager::isPlugin ( const std::string &name )
{
   return isPlugin( name.c_str() );
}

inline bool PluginManager::load ( const std::string &plugin_name, const bool init )
{
   return load( plugin_name.c_str(), init );
}

inline Plugin* PluginManager::loadAndGetPlugin ( const std::string &plugin_name, const bool init )
{
   return loadAndGetPlugin( plugin_name.c_str(), init );
}

#endif
