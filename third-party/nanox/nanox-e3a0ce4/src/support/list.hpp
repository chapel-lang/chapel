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

#ifndef _NANOS_LIST
#define _NANOS_LIST

#include "atomic.hpp"
#include <list>
#include <limits.h>
#include <iterator>
#include <stddef.h>

using namespace nanos;

template<class _T>
inline List<_T>::~List()
{
   ListNode *it = _begin;
   while ( it != _end ) {
      ListNode* tmp = it;
      it = it->next();
      delete tmp;
   }
}

template<class _T>
inline _T& List<_T>::front()
{ return *_begin->getItem(); }

template<class _T>
inline _T& List<_T>::push_front( const _T& elem )
{
   ListNode *node;
   if (_freeList.empty() ) {
      node = new ListNode( elem );
      node->setNext( _begin );
      _begin = node;
      node->setValid( true );
   } else {
      node = _freeList.front();
      _freeList.pop_front();
      node->setItem( elem );
      node->setRefs(0);
      node->setValid( true );
   }
   _size++;
   return node->getItem();
}

template<class _T>
inline void List<_T>::pop_front()
{
   // Get an iterator to the first valid element
   iterator it = begin();

   while ( it != end() ) {
      // Invalidate the element
      ListNode *elem = it._node;
      if ( elem->setRefsCswap(_N) ) {
         elem->setValid(false);
         _freeList.push_back(elem);
         _size--;
         return;
      }
      it++;
   }
}

template<class _T>
inline bool List<_T>::erase( const _T& t )
{
   iterator it = begin();
   while ( it!= end()) {
      if ( t == *it ) {
         ListNode *elem = it._node;
         if ( elem->setRefsCswap(_N) ) {
            elem->setValid(false);
            _freeList.push_back(elem);
            _size--;
            return true;
         }
      return false;
      }
      it++;
   }
   return false;
}

template<class _T>
inline typename List<_T>::iterator List<_T>::begin()
{
   iterator it(_begin, this);
   it.skip();
   return it;
}

template<class _T>
inline typename List<_T>::iterator List<_T>::end()
{
   return iterator(_end, this);
}

template<class _T>
inline size_t List<_T>::size() const
{
   return _size;
}

template<class _T>
inline Lock& List<_T>::getLock ( )
{
   return _lock;
}
/*
template<class _T>
inline void List<_T>::lock ( )
{
   _lock.acquire();
   memoryFence();
}

template<class _T>
inline void List<_T>::unlock ( )
{
   memoryFence();
   _lock.release();
}
*/
#endif

