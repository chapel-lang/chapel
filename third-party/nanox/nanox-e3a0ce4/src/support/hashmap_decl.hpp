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

#ifndef _NANOS_HASH_MAP_DECL
#define _NANOS_HASH_MAP_DECL

#define USE_NANOS_LIST 1

#include "list_decl.hpp"
#include <list>
#include <map>
#include "atomic_decl.hpp"


namespace nanos {

/*! \class Hash
 *  \brief Hash function template
 */
template<typename _KeyType>
class Hash
{
   public:
     /*! \brief Default constructor
      */
      Hash() {}
     /*! \brief Destructor
      */
      virtual ~Hash() {}

     /*! \brief Hash operation for the given key and map size
      *  \param key compute the hash value for this key
      *  \param size size of the map
      */
      virtual size_t operator()( _KeyType key, size_t size )
         { return ((size_t)key) % size; }
};


/*! \class HashMap
 *  \brief HashMap that uses the List that allows concurrent access and deletion of entries
 */
template <typename _KeyType, typename _T, bool _invalidate = false, size_t _tsize = 256, typename _HashFunction = Hash<_KeyType> >
class HashMap
{
   private:

     /*! \class MapEntry
      *  \brief Bucket that stores an element in the map
      */
      class MapEntry
      {
         private:
            _KeyType _key; /**< Element's key */
            _T _value;     /**< Value stored in the Map */
            unsigned int _lru; /**< Counter for the LRU policy */

           /*! \brief Default constructor
            */
            MapEntry() {}

         public:
           /*!  \brief Constructor
            *   \param k key for an entry with the value initialized by default
            */
            MapEntry( _KeyType k ) : _key(k), _value(), _lru(0) {}

           /*!  \brief Constructor
            *   \param k key for the entry
            *   \param v value
            */
            MapEntry( _KeyType k, _T v ) : _key(k), _value(v), _lru(0) {}

           /*!  \brief Copy constructor
            *   \param e another MapEntry
            */
            MapEntry( MapEntry const &e ) : _key(e._key), _value(e._value), _lru(0) {}

           /*!  \brief Assignation operator
            *   \param e another MapEntry
            */
            MapEntry& operator=( MapEntry const &e )
            {
               if (this == &e) return *this;
               _key = e._key;
               _value = e._value;
               return *this;
            }

           /*! \brief Compare two MapEntries
            *   \param e another MapEntry
            */
            bool operator==( MapEntry const &e ) const
            {
               return _key == e._key;
            }

           /*! \breif Returns the key of the entry
            */
            _KeyType& getKey()
               { return _key; }

           /*! \brief Returns the value of the entry
            */
            _T& getValue()
               { return _value; }

           /*! \brief Set the LRU counter for this entry
            *  \param val Set the counter to val
            */
            void setLRU( unsigned int val )
               { _lru = val; }

           /*! \brief Returns the LRU counter
            */
            unsigned int getLRU()
               { return _lru; }
      };

#ifdef USE_NANOS_LIST
      typedef List<MapEntry> HashList; /**< List Used buy the HashMap */
#else
      typedef std::list<_T> HashList; /**< Stl list used for debugging the HashMap */
#endif

   public:

     /*! \class iterator
      *  \brief HashMap's iterator
      */
      class iterator {
         private:
            friend class HashMap;
            HashMap& _map; /**< Map whose elements this iterator points to */

            int _currentTable; /**< Table of the current table of the element the iterator points to */

            typename HashList::iterator _currentItem; /**< Current item pointed by the iterator */

           /*! \brief Skip empty tables in the Map
            */
            void skip()
            {
                  if ( _currentTable == -1 )
                     return;
                  while ( _currentItem == _map._table[_currentTable].end() && _currentTable + 1 < (int)_map._tableSize ) { 
                     _currentItem = _map._table[++_currentTable].begin();
                  }

                  if ( _currentItem == _map._table[_currentTable].end() ) {
                     _currentTable = -1;
                  }
            }

           /*! \brief Default constructor
            */
            iterator() {}

         public:
           /*! \brief Constructor
            * \param hm HashMap for this iterator
            * \param currT Index of the table of the item to initialize the iterator
            * \param item Current item pointed by the iterator
            */
            iterator( HashMap &hm, int currT, typename HashList::iterator item ) : _map(hm), _currentTable( currT ), _currentItem( item )
            {
               if ( currT != -1 ) skip();
            }

           /*! \brief Copy constructor
            *  \param it another iterator
            */
            iterator( iterator const &it ) : _map( it._map ), _currentTable( it._currentTable ), _currentItem( it._currentItem ) {}

           /*! \brief Destructor
            */
            ~iterator() {}

           /*! \brief Assign operator
            *  \param it another iterator
            */
            iterator const& operator=( iterator const &it )
            {
               _map = it._map;
               _currentTable = it._currentTable;
               _currentItem = it._currentItem;
               return *this;
            }

           /*! \brief Next element
            */
            iterator operator++( int unused )
            {
               if ( _currentTable != -1 ) {
                  _currentItem++;
                  skip();
               }
               return *this;
            }

           /*! \brief Next element
            */
            iterator operator++()
            {
               if ( _currentTable != -1 ) {
                  _currentItem++;
                  while ( _currentTable < _map.tableSize && _currentItem == _map._table[_currentTable].end() ) {
                     _currentTable++;
                     _currentItem = _map._table[_currentTable].begin();
                  }
                  if ( _currentTable = _map._tableSize ) {
                     _currentTable = -1;
                  }
               }
               return *this;
            }

           /*! \brief Returns a reference to the element in the Map pointed by the iterator
            */
            _T const& operator*() const
               { return _currentItem->getValue() ; }

           /*! \brief Returns a reference to the element in the Map pointed by the iterator
            */
            _T& operator*()
               { return _currentItem->getValue(); }

           /*! \brief Returns a pointer to the element in the Map pointed by the iterator
            */
            _T const * operator->() const
               { return &(_currentItem->getValue()); }

           /*! \brief Returns a pointer to the element in the Map pointed by the iterator
            */
            _T* operator->()
               { return &(_currentItem->getValue()); }

          /*! \brief copmpare two iterators
           */
           bool operator==( iterator const &it ) const
              { return (_currentTable == -1 && it._currentTable == -1) || ( _currentTable == it._currentTable && _currentItem == it._currentItem); }

          /*! \brief copmpare two iterators
           */
           bool operator!=( iterator const &it ) const
              { return !((_currentTable == -1 && it._currentTable == -1) || ( _currentTable == it._currentTable && _currentItem == it._currentItem)); }
      };

   private:
      size_t _tableSize; /**< size of the table. */
      HashList _table[_tsize]; /**< Table of lists of entries */
      _HashFunction _hash; /**< Hash function used by the map */

      Atomic<unsigned int> _lruCounter; /**< Global counter for the LRU policy */

     /*! \brief Copy constructor
      */
      HashMap( HashMap const &map ) {}

     /*! \brief Assingation operator
      */
      HashMap const& operator=( HashMap const &map ) { return *this; }

   public:
      typedef std::list<_T> ItemList; /**< Type for a list of items */
      typedef std::map<unsigned int, _KeyType> KeyList; /**< Type for a list of Keys*/

     /*! \brief Default constructor
      */
      HashMap() : _tableSize(_tsize), _table(), _hash(), _lruCounter(0) {}

     /*! \brief Destructor
      */
      ~HashMap() {}

     /* \brief Looks up for an element in the hash, if none fouund, it creates it.
      * Thread safe if the user holds a reference on the element, the element may be deteted after the call
      */
      _T& operator[]( _KeyType key );

     /* \brief Looks up for an element in the hash
      * Thread safe if the user holds a reference on the element otherwise, the element may be deleted after the call
      */
      _T* find( _KeyType key );

     /* \brief Equivalet to operator[] but atomically increases references
      */
      _T& accessAndReference( _KeyType key );

     /* \brief Inserts atomically the given element with the given key. Sets inserted to true if so,
      *        and returns a reference to the inserted element.
      */
      _T& insert( _KeyType key, _T& elem, bool& inserted );

     /* \brief Equivalet to find but atomically increases references
      */
      _T* findAndReference( _KeyType key );

     /* \brief Decreases references
      */
      void deleteReference( _KeyType key );

     /* \brief Returns the number of references
      */
      unsigned int getReferenceCount( _KeyType key );

     /* \brief Tries to erase the element identified by 'key' and returns true if successful (it had no references)
      */
      bool erase( _KeyType key );

     /* \brief Retrns a list of keys corresponding to entries with no references
      */
      void listUnreferencedKeys( KeyList& unreferenced );

     /* \brief Removes all elements in the hash, this is not thread safe
      */
      void flush( ItemList& removedItems );

     /* \brief Retruns an iterator to the begin of the map
      */
      iterator begin() {
        return iterator( *this, 0, _table[0].begin() );
      }

     /* \brief Returns an iterator to the end of the map
      */
      iterator end() {
        return iterator( *this, -1, _table[0].end() );
      }
};

}

#endif

