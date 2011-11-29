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

#ifndef _NANOS_SIMPLEALLOCATOR_DECL
#define _NANOS_SIMPLEALLOCATOR_DECL

#include <stdint.h>
#include <map>

namespace nanos {

   /*! \brief Simple memory allocator to manage a given contiguous memory area
    */
   class SimpleAllocator
   {
      private:
         typedef std::map < uint64_t, size_t > SegmentMap;

         SegmentMap _allocatedChunks;
         SegmentMap _freeChunks;

         uint64_t _baseAddress;

      public:
         SimpleAllocator( uint64_t baseAddress, size_t len );

         // WARNING: Calling this constructor requires calling init() at some time
         // before any allocate() or free() methods are called
         SimpleAllocator() : _baseAddress( 0 ) { }

         void init( uint64_t baseAddress, size_t len );

         void * allocate( size_t len );

         uint64_t getBaseAddress ();

         size_t free( void *address );

         void printMap();
   };

}
#endif /* _NANOS_SIMPLEALLOCATOR */
