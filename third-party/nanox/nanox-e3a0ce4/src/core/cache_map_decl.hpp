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

#ifndef _NANOS_CACHE_MAP_DECL_H
#define _NANOS_CACHE_MAP_DECL_H

#include "compatibility.hpp"
#include "atomic_decl.hpp"
#include "cache_fwd.hpp"

namespace nanos
{

  /*! \class CacheMap
   *  \brief Builds a map of the caches to be used by the directories to identify data locality
   */
   class CacheMap
   {
      private:
         Atomic<unsigned int> _numCaches; /**< Number of caches registered by the system in the map */

         /*! \brief CacheMap copy constructor (private)
          */
         CacheMap( const CacheMap &map );

         /*! \brief CacheMap copy assignment operator (private)
          */
         const CacheMap & operator= ( const CacheMap &map );

      public:

         /*! \brief CacheMap default constructor
          */
         CacheMap() : _numCaches(1) { }

         /*! \brief CacheMap destructor
          */
         ~CacheMap() { }

        /*! \brief Register a new Cache in the map
         *  \return Returns an identifier for the cache
         */
         unsigned int registerCache();

        /*! \brief Returns the number of caches registered in the system
         */
         unsigned int getSize() const;
   };

   class CacheAccessMap
   {
      private:
         unsigned int _size;
         Atomic<unsigned int> *_cacheAccessesById;
      public:
         CacheAccessMap( unsigned int size );

         ~CacheAccessMap();

        /*! \brief CacheAccessMap copy constructor (private)
         */
         CacheAccessMap( const CacheAccessMap &map );

        /*! \brief CacheAccessMap copy assignment operator (private)
         */
         const CacheAccessMap & operator= ( const CacheAccessMap &map );

        /*! \brief Returns a reference to the cache's counter
         */
         Atomic<unsigned int> & operator[] ( unsigned int cacheId );

        /*! \brief Returns the number of accesses for 'cache'
         */
         unsigned int getAccesses( unsigned int cacheId );
   };

};

#endif
