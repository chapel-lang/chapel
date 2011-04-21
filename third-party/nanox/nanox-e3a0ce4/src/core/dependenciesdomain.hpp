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

#ifndef _NANOS_DEPENDENCIES_DOMAIN
#define _NANOS_DEPENDENCIES_DOMAIN
#include <stdlib.h>
#include <map>
#include <list>
#include <vector>
#include "dependenciesdomain_decl.hpp"
#include "atomic.hpp"
#include "dependableobject.hpp"
#include "trackableobject.hpp"
#include "dependency.hpp"
#include "compatibility.hpp"


using namespace nanos;

inline DependenciesDomain::~DependenciesDomain ( )
{
   for ( DepsMap::iterator it = _addressDependencyMap.begin(); it != _addressDependencyMap.end(); it++ ) {
      delete it->second;
   }
}

inline void DependenciesDomain::submitDependableObject ( DependableObject &depObj, std::vector<Dependency> &deps )
{
   submitDependableObjectInternal ( depObj, deps.begin(), deps.end() );
}

inline void DependenciesDomain::submitDependableObject ( DependableObject &depObj, size_t numDeps, Dependency* deps)
{
   submitDependableObjectInternal ( depObj, deps, deps+numDeps );
}

inline void DependenciesDomain::lock ( )
{
   _lock.acquire();
   memoryFence();
}

inline void DependenciesDomain::unlock ( )
{
   memoryFence();
   _lock.release();
}

#endif

