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

#ifndef _NANOS_HASH_MAP
#define _NANOS_HASH_MAP

#include "hashmap_decl.hpp"
#include "list.hpp"
#include <algorithm>

using namespace nanos;

template <typename _KeyType, typename _T, bool _invalidate, size_t _tsize, typename _HashFunction>
_T& HashMap<_KeyType,_T,_invalidate,_tsize,_HashFunction>::operator[]( _KeyType key )
{
   MapEntry * entry;
   HashList& list = _table[_hash(key,_tsize)];
   typename HashList::iterator it = std::find( list.begin(), list.end(), MapEntry(key) );
   if ( it == list.end() ) {
      {
         SyncLockBlock lock( list.getLock() );
         it = std::find( list.begin(), list.end(), MapEntry(key) );
         if ( it == list.end() ) {
            entry = &(list.push_front(MapEntry(key)));
         } else {
            entry = &(*it);
         }
      }
   } else {
      entry = &(*it);
   }
   return entry->getValue();
}

template <typename _KeyType, typename _T, bool _invalidate, size_t _tsize, typename _HashFunction>
_T& HashMap<_KeyType,_T,_invalidate,_tsize,_HashFunction>::insert( _KeyType key, _T& elem, bool& inserted )
{
   inserted = false;
   MapEntry *entry;
   HashList& list = _table[_hash(key,_tsize)];
   {
      SyncLockBlock lock( list.getLock() );
      typename HashList::iterator it = std::find( list.begin(), list.end(), MapEntry(key) );
      if ( it == list.end() ) {
         entry = &(list.push_front(MapEntry(key,elem)));
         it = std::find( list.begin(), list.end(), MapEntry(key) );
         it.addReference();
         inserted = true;
      } else {
         entry = &(*it);
         it.addReference();
      }
   }
   return (entry->getValue());
}

template <typename _KeyType, typename _T, bool _invalidate, size_t _tsize, typename _HashFunction>
_T* HashMap<_KeyType,_T,_invalidate,_tsize,_HashFunction>::find( _KeyType key )
{
   HashList& list = _table[_hash(key,_tsize)];
   typename HashList::iterator it = std::find( list.begin(), list.end(), MapEntry(key) );
   if ( it == list.end() ) {
   	return NULL;
   }
   MapEntry& entry = *it;
   return &(entry.getValue());
}

template <typename _KeyType, typename _T, bool _invalidate, size_t _tsize, typename _HashFunction>
_T* HashMap<_KeyType,_T,_invalidate,_tsize,_HashFunction>::findAndReference( _KeyType key )
{
   HashList& list = _table[_hash(key,_tsize)];
   typename HashList::iterator it = std::find( list.begin(), list.end(), MapEntry(key) );
   if ( it == list.end() ) {
   	return NULL;
   }
   it.addReference();
   MapEntry& entry = *it;
   return &(entry.getValue());
}

template <typename _KeyType, typename _T, bool _invalidate, size_t _tsize, typename _HashFunction>
void HashMap<_KeyType,_T,_invalidate,_tsize,_HashFunction>::deleteReference( _KeyType key )
{
   HashList& list = _table[_hash(key,_tsize)];
   typename HashList::iterator it = std::find( list.begin(), list.end(), MapEntry(key) );
   if ( it == list.end() ) {
   	return;
   }
   it.deleteReference();
   MapEntry& entry = *it;
   if ( it.getReferences() == 1 )
      entry.setLRU( _lruCounter++ );
}

template <typename _KeyType, typename _T, bool _invalidate, size_t _tsize, typename _HashFunction>
unsigned int HashMap<_KeyType,_T,_invalidate,_tsize,_HashFunction>::getReferenceCount( _KeyType key )
{
   HashList& list = _table[_hash(key,_tsize)];
   typename HashList::iterator it = std::find( list.begin(), list.end(), MapEntry(key) );
   if ( it == list.end() ) {
      return 0;
   }
   return it.getReferences() - 1; // substract the one hold by the iterator
}

template <typename _KeyType, typename _T, bool _invalidate, size_t _tsize, typename _HashFunction>
_T& HashMap<_KeyType,_T,_invalidate,_tsize,_HashFunction>::accessAndReference( _KeyType key )
{
   HashList& list = _table[_hash(key,_tsize)];
   typename HashList::iterator it = std::find( list.begin(), list.end(), MapEntry(key) );
   if ( it == list.end() ) {
      list.push_front(MapEntry(key));
      it = std::find( list.begin(), list.end(), MapEntry(key) );
   }
   it.addReference();
   MapEntry& entry = *it;
   return entry.getValue();
}



template <typename _KeyType, typename _T, bool _invalidate, size_t _tsize, typename _HashFunction>
bool HashMap<_KeyType,_T,_invalidate,_tsize,_HashFunction>::erase( _KeyType key )
{
   HashList& list = _table[_hash(key,_tsize)];
   return list.erase(MapEntry(key));
}

template <typename _KeyType, typename _T, bool _invalidate, size_t _tsize, typename _HashFunction>
void HashMap<_KeyType,_T,_invalidate,_tsize,_HashFunction>::listUnreferencedKeys( KeyList& unreferenced )
{
   for (size_t i=0; i < _tsize; i++) {
      typename HashList::iterator it = _table[i].begin();
      while ( it != _table[i].end() ) {
         if ( it.getReferences() == 1 ) {
            MapEntry& entry = *it;
            unreferenced.insert( std::pair<unsigned int, _KeyType>( entry.getLRU(), entry.getKey() ) );
         }
         it++;
      }
   }
}

template <typename _KeyType, typename _T, bool _invalidate, size_t _tsize, typename _HashFunction>
void HashMap<_KeyType,_T,_invalidate,_tsize,_HashFunction>::flush( ItemList& removedItems )
{
   for (size_t i=0; i < _tsize; i++) {
      while ( _table[i].size() > 0 ) {
         removedItems.add(_table[i].front());
         _table[i].pop_front();
      }
   }
}

#endif

