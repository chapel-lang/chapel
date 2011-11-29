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
#ifndef _NANOS_CACHE_MAP_HPP
#define _NANOS_CACHE_MAP_HPP
#include "cache_map_decl.hpp"
#include "new_decl.hpp"

using namespace nanos;

inline unsigned int CacheMap::registerCache()
{
   return _numCaches++;
}

inline unsigned int CacheMap::getSize() const
{
   return _numCaches.value() - 1;
}

inline CacheAccessMap::CacheAccessMap( unsigned int size ) : _size(size)
{
   _cacheAccessesById = NEW Atomic<unsigned int>[size];
}

inline CacheAccessMap::~CacheAccessMap()
{
   delete[] _cacheAccessesById;
}

inline CacheAccessMap::CacheAccessMap( const CacheAccessMap &map ) : _size( map._size )
{
   if ( this == &map )
      return;
   _cacheAccessesById = NEW Atomic<unsigned int>[_size];
   for ( unsigned int i = 0; i < _size; i++ ) {
      _cacheAccessesById[i] = map._cacheAccessesById[i];
   }
}

inline const CacheAccessMap& CacheAccessMap::operator= ( const CacheAccessMap &map )
{
   if ( this == &map )
      return *this;
   _size = map._size;
   _cacheAccessesById = NEW Atomic<unsigned int>[_size];
   for ( unsigned int i = 0; i < _size; i++ ) {
      _cacheAccessesById[i] = map._cacheAccessesById[i];
   }
}

inline Atomic<unsigned int>& CacheAccessMap::operator[] ( unsigned int cacheId )
{
   return _cacheAccessesById[cacheId - 1];
}

inline unsigned int CacheAccessMap::getAccesses( unsigned int cacheId )
{
   return _cacheAccessesById[cacheId - 1].value();
}

#endif
