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
#ifndef _NANOS_ALLOCATOR_HPP
#define _NANOS_ALLOCATOR_HPP
#include "allocator_decl.hpp"
#include "debug.hpp"
#include "basethread.hpp"
#include <vector>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace nanos {

extern Allocator *allocator;

inline Allocator & getAllocator ( void )
{
   if (!allocator) {
      allocator = (Allocator *) malloc(sizeof(Allocator));
      new (allocator) Allocator();
   }

   BaseThread *my_thread = getMyThreadSafe();
   if ( my_thread == NULL ) return *allocator;
   else return my_thread->getAllocator();
}

inline size_t Allocator::Arena::getObjectSize () const
{
   return _objectSize;
}

inline void * Allocator::Arena::allocate ( void )
{
   unsigned int obj;

   if ( !_free ) return NULL;

   for ( obj = 0 ; obj < numObjects ; obj++ ) {
      if ( _bitmap[obj]._bit ) break;
   }

   if (obj == numObjects) {
      _free = false;
      return NULL; 
   }

   _bitmap[obj]._bit = false;

   return (void *) &_arena[obj*_objectSize];
}

inline void Allocator::Arena::deallocate ( void *object )
{
   unsigned long offset = ((char *) object - _arena);
   unsigned long index = offset / _objectSize;

   _free = true;
   _bitmap[index]._bit = true;
}

inline Allocator::Arena * Allocator::Arena::getNext ( void ) const
{
   return _next;
}

inline void Allocator::Arena::setNext ( Arena * a )
{
   _next = a;
}

inline void * Allocator::allocate ( size_t size, const char* file, int line )
{
   /* realSize is (size + header )'s next power of 2 */
   size_t realSize = size + _headerSize + 1;
   realSize |= realSize >> 1;
   realSize |= realSize >> 2;
   realSize |= realSize >> 4;
   realSize |= realSize >> 8;
   realSize |= realSize >> 16;
   realSize++;
 

   ArenaCollection::iterator it;
   for ( it = _arenas.begin(); it != _arenas.end(); it++ ) {
      if ( (*it)->getObjectSize() == realSize ) break;
   }

   Arena *arena = NULL;

   if ( it == _arenas.end() ) {
      // no arena found for that size, create a new one
      arena = (Arena *) malloc ( sizeof(Arena) );
      new ( arena ) Arena( realSize );
      _arenas.push_back( arena );
   }
   else arena = *it;


   ObjectHeader * ptr = NULL;

   while ( ptr == NULL ) {
      ptr = (ObjectHeader *) arena->allocate();
      if ( ptr == NULL ) { 
          if ( arena->getNext() == NULL ) {
             Arena *next = ( Arena *) malloc ( sizeof(Arena) );
             new (next) Arena(realSize);
             arena->setNext( next );
          }
          arena = arena->getNext(); 
      }
   }

   ptr->_arena = arena;

   return  ((char *) ptr ) + _headerSize;
}

inline void Allocator::deallocate ( void *object, const char *file, int line )
{
   ObjectHeader * ptr = (ObjectHeader *) ( ((char *)object) - _headerSize );
   Arena *arena = ptr->_arena;
   arena->deallocate(ptr);
}

} // namespace nanos

#endif
