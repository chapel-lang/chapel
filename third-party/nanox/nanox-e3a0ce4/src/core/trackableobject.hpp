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

#ifndef _NANOS_TRACKABLE_OBJECT_H
#define _NANOS_TRACKABLE_OBJECT_H
#include <stdlib.h>
#include <list>
#include <algorithm>
#include "trackableobject_decl.hpp"
#include "dependableobject.hpp"
#include "atomic.hpp"

using namespace nanos;

inline const TrackableObject & TrackableObject::operator= ( const TrackableObject &obj )
{
   _address = obj._address;
   _lastWriter = obj._lastWriter;
   return *this;
}

inline void * TrackableObject::getAddress ( )
{
   return _address;
}

inline bool TrackableObject::hasLastWriter ( )
{
   return _lastWriter != NULL;
}

inline DependableObject* TrackableObject::getLastWriter ( )
{
   return _lastWriter;
}

inline void TrackableObject::setLastWriter ( DependableObject &depObj )
{
   {
      SyncLockBlock lock( _writerLock );
      _lastWriter = &depObj;
   }
}

inline void TrackableObject::deleteLastWriter ( DependableObject &depObj )
{
   if ( _lastWriter == &depObj ) {

      {
         SyncLockBlock lock( _writerLock );
         if ( _lastWriter ==  &depObj ) {
            _lastWriter = NULL;
         }
      }
   }
}

inline TrackableObject::DependableObjectList & TrackableObject::getReaders ( )
{
   return _versionReaders;
}

inline void TrackableObject::setReader ( DependableObject &reader )
{
   _versionReaders.push_back( &reader );
}

inline bool TrackableObject::hasReader ( DependableObject &depObj )
{
   return ( find( _versionReaders.begin(), _versionReaders.end(), &depObj ) != _versionReaders.end() );
}

inline void TrackableObject::flushReaders ( )
{
   _versionReaders.clear();
}

inline void TrackableObject::deleteReader ( DependableObject &reader )
{
   _versionReaders.remove( &reader );
}

inline bool TrackableObject::hasReaders ()
{
   return !( _versionReaders.empty() );
}

inline Lock& TrackableObject::getReadersLock()
{
   return _readersLock;
}

inline CommutationDO* TrackableObject::getCommDO()
{
   return _commDO;
}

inline void TrackableObject::setCommDO( CommutationDO *commDO )
{
   _commDO = commDO;
}

#endif
