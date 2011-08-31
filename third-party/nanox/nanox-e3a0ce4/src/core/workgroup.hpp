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

#ifndef _NANOS_WORK_GROUP_H
#define _NANOS_WORK_GROUP_H

#include "workgroup_decl.hpp"
#include "atomic.hpp"
#include "schedule.hpp"
#include "synchronizedcondition.hpp"
#include "system.hpp"
#include "instrumentation.hpp"
#include "workdescriptor_decl.hpp"

using namespace nanos;

inline WorkGroup::WorkGroup( const WorkGroup &wg ) : _id( _atomicSeed++ ), _components( 0 ), _phaseCounter( 0 ),
            _syncCond( EqualConditionChecker<int>(&_components.override(), 0 ) ), _parent(NULL)  
{
   if ( wg._parent != NULL ) { 
      wg._parent->addWork(*this);
      for ( WGList::const_iterator it = wg._partOf.begin(); it < wg._partOf.end(); it++ ) {
         if (*it) (*it)->addWork( *this );
      }
   }
}

#endif

