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

#ifndef _NANOS_LIST_DECL
#define _NANOS_LIST_DECL

#include "atomic_decl.hpp"
#include <list>
#include <limits.h>
#include <iterator>
#include <stddef.h>

namespace nanos {

/*! \class List
 *  \brief This list is tuned to erase and read elements without using locks
 *  compatible with std algorithms (actually only std::find has been tested
 */
template<class _T>
class List {
   public:

     /*! \class ListNode
      * \brief List internal structure that stores the actual elements
      */
      class ListNode {
         private:
            _T             _object; /**< Element stored in this node */
            ListNode*     _next; /**< Pointer to next node in the list */
            Atomic<int>   _refs; /**< Atomic references counter */
            volatile bool _valid; /**< Valid flag */

         public:
           /*! \brief Default constructor
            */
            ListNode() : _object(), _next(NULL), _refs(0), _valid(false) {}

           /*! \brief Constructor
            *  \param object Element to initialize this node with
            *  \param next Next node in the list (if any)
            */
            ListNode( _T &object, ListNode* next = NULL ) : _object(object), _next(next), _refs(0), _valid(false) {}

           /*! \brief Constructor
            *  \param object Element to initialize this node with
            *  \param next Next node in the list (if any)
            */
            ListNode( const _T &object, ListNode* next = NULL ) : _object(object), _next(next), _refs(0), _valid(false) {}

           /*! \brief Copy constructor
            *  \param node Another list node
            */
            ListNode( ListNode const &node ) : _object(node._object), _next(node._next), _refs(node._refs), _valid(node._valid) {}

           /*! \brief Copy operator
            *  \param node Another list node
            */
            ListNode& operator=( ListNode const &node)
            {
               if (this == &node) return *this;
               this._object = node._object;
               this._next = node._next;
               return *this;
            }

           /*! \brief Returns the element in this node (const reference)
            */
            const _T& getItem() const
               { return _object; }

           /*! \brief Returns the element in this node (reference)
            */
            _T& getItem()
               { return _object; }

           /*! \brief Assigns to the node's element the passed one
            *  \param item
            */
            void setItem( _T& item )
               { _object = item; }

           /*! \brief Assigns to the node's element the passed one
            *  \param item (const)
            */
            void setItem( const _T& item )
               { _object = item; }

           /*! \brief Returns the next node in the list
            */
            ListNode* next() const
               { return _next; }

           /*! \brief Sets the next node in the list
            * \param next Another list node
            */
            void setNext( ListNode* next )
               { _next = next; }

           /*! \brief Returns whether this node has external references or not
            */
            bool hasRefs() const
               { return _refs > 0; }

           /*! \brief Returns the external reference counter
            */
            int getRefs() const
               { return _refs.value(); }

           /*! \brief Set the number of external references to refs
            *  \param refs
            */
            void setRefs( int refs )
               { _refs = refs; }

           /*! \brief Increases the reference counter by one
            */
            int increaseRefs()
               { return _refs++; }

           /*! \brief Decreases the reference counter by one
            */
            int decreaseRefs()
               { return ( _refs.value() <= 0 ? _refs.value() : _refs--); }

           /*! \brief Set conditionally reference counter to val if it was 1
            *  \param val
            */
            bool setRefsCswap( int val )
            {
               Atomic<int> putVal = val;
//               Atomic<int> expected = 0;
               Atomic<int> expected = 1; // the last reference will always be the iterator itself
               return _refs.cswap( expected, putVal );
            }

           /*! \brief returns whether the 
            */
            bool isValid() const
               { return _valid; }

           /*! \brief Set valid flag to 'valid'
            *  \param valid
            */
            void setValid( bool valid )
               { _valid = valid; }
      };

      class const_iterator;

     /*! \class iterator
      *  \brief list iterator
      *  Note that the iterator itself holds a reference
      */
      class iterator {
         private:
            ListNode*   _node; /**< List node */
            List*       _list; /**< List that the iterator belongs to */

           /*! \brief Default constructor disabled
            */
            iterator();

            friend class List;
            friend class const_iterator;

         public:
            typedef std::forward_iterator_tag iterator_category; /**< compatibility with stl iterators */
            typedef _T                   value_type; /**< compatibility with stl iterators */
            typedef ptrdiff_t            difference_type; /**< compatibility with stl iterators */
            typedef _T*                  pointer; /**< compatibility with stl iterators */
            typedef _T&                  reference; /**< compatibility with stl iterators */

           /*! \brief Constructor
            *  \param node
            *  \param list
            */
            iterator( ListNode* node, List* list ) : _node(node), _list(list) { }

           /*! \brief Copy constructor
            *  \param it Another iterator
            */
            iterator( iterator const &it ) : _node(it._node), _list(it._list)
            {
               if ( _node != _list->_end ) {
                  _node->increaseRefs();
               }
            }

           /*! \brief iterator's destructor
            */
            ~iterator()
            {
               if ( _node != _list->_end ) {
                  _node->decreaseRefs();
               }
            }

           /*! \brief Cast to const_iterator
            */
            operator const_iterator () const
               { return const_iterator( _node, _list ); }

           /*! \brief Assign operator
            *  \param it Another iterator
            */
            iterator const& operator=( iterator const &it )
            {
               if ( _node != _list->_end )
                  _node->decreaseRefs();
               _node = it._node;
               _list = it._list;
               if ( _node != _list->_end )
                  _node->increaseRefs();
               return *this;
            }

           /*! \brief Skips invalid elements in the list
            */
            void skip()
            {
               while ( _node != _list->_end ) {
                  if ( _node->isValid() ) {
                     if ( _node->increaseRefs() >= 0 ) {
                        if ( _node->isValid() ) {
                           return;
                        }
                     }
                     _node->decreaseRefs();
                  }
                  _node = _node->next();
               }
            }

           /*! \brief Next element in the list
            */
            iterator operator++( int unused )
            {
               iterator result = iterator(*this);
               if ( _node != _list->_end ) {
                  _node->decreaseRefs();
                  _node = _node->next();
                  if ( _node != _list->_end ) {
                     skip();
                  }
               }
               return result;
            }

           /*! \brief Next element in the list
            */
            iterator operator++()
            {
               if ( _node != _list->_end ) {
                  _node->decreaseRefs();
                  _node = _node->next();
                  if ( _node != _list->_end ) {
                     skip();
                  }
               }
               return *this;
            }

          /*! \brief derreference operator (const)
           */
           _T const& operator*() const
              { return _node->getItem(); } 

          /*! \brief derreference operator
           */
           _T& operator*()
              { return _node->getItem(); }

          /*! \brief arrow operator (const)
           */
           _T const * operator->() const
              { return &(_node->getItem()); }

          /*! \brief arrow operator
           */
           _T* operator->()
              { return &(_node->getItem()); }

          /*! \brief iterator compatation
           *  \param it Another iterator
           */
           bool operator==( iterator const &it ) const
              { return _node == it._node; }

          /*! \brief iterator compatation
           *  \param it Another iterator
           */
           bool operator!=( iterator const &it ) const
              { return _node != it._node; }

          /*! \brief iterator compatation
           *  \param it const_iterator
           */
           bool operator==( const_iterator const &it ) const
              { return _node == it._node; }

          /*! \brief iterator compatation
           *  \param it const_iterator
           */
           bool operator!=( const_iterator const &it ) const
              { return _node != it._node; }

          /*! \brief Returns the number of references of the internal node
           */
           int getReferences() const
           {
              if (_node != _list->_end ) {
                 return _node->getRefs();
              }
              return 0;
           }

          /*! \brief Increases the number of references of the internal node
           */
           void addReference()
           {
              if ( _node != _list->_end ) {
                 _node->increaseRefs();
              }
           }

          /*! \brief Decreases the number of references of the internal node
           */
           void deleteReference()
           {
              if ( _node != _list->_end ) {
                 _node->decreaseRefs();
              }
           }
      };

     /*! \class const_iterator
      *  \brief list iterator
      *  Note that the iterator itself holds a reference
      */
      class const_iterator {
         private:
            ListNode*   _node; /**< List node */
            List*       _list; /**< List that the iterator belongs to */

           /*! \brief Default constructor disabled
            */
            const_iterator();

            friend class List;
            friend class iterator;

         public:
            typedef std::forward_iterator_tag iterator_category; /**< compatibility with stl iterators */
            typedef _T                   value_type; /**< compatibility with stl iterators */
            typedef ptrdiff_t            difference_type; /**< compatibility with stl iterators */
            typedef _T*                  pointer; /**< compatibility with stl iterators */

           /*! \brief Constructor
            *  \param node
            *  \param list
            */
            const_iterator( ListNode* node, List* list ) : _node(node), _list(list) {}

           /*! \brief Copy constructor
            *  \param it Another const_iterator
            */
            const_iterator( const_iterator const &it ) : _node(it._node), _list(it._list) {}

           /*! \brief const_iterator's destructor
            */
            ~const_iterator()
            {
               if ( _node != _list->_end )
                  _node->decreaseRefs();
            }

           /*! \brief Cast to const_iterator
            */
            operator const_iterator () const
               { return const_iterator( _node, _list ); }

           /*! \brief Skips invalid elements in the list
            */
            void skip()
            {
               while ( _node != _list->_end ) {
                  if ( _node->isValid() ) {
                     if ( _node->increaseRefs() >= 0 ) {
                        if ( _node->isValid() ) {
                           return;
                        }
                     }
                     _node->decreaseRefs();
                  }
                  _node = _node->next();
               }
            }

           /*! \brief Next element in the list
            */
            const_iterator operator++( int unused )
            {
               const_iterator result = const_iterator(*this);
               if ( _node != _list->_end ) {
                  _node->decreaseRefs();
                  _node = _node->next();
                  if ( _node != _list->_end ) {
                     skip();
                  }
               }
               return result;
            }

           /*! \brief Next element in the list
            */
            const_iterator operator++()
            {
               if ( _node != _list->_end ) {
                  _node->decreaseRefs();
                  _node = _node->next();
                  if ( _node != _list->_end ) {
                     skip();
                  }
               }
               return *this;
            }

          /*! \brief derreference operator (const)
           */
           _T const& operator*() const
              { return _node->getItem(); } 

          /*! \brief derreference operator
           */
           _T& operator*()
              { return _node->getItem(); }

          /*! \brief arrow operator (const)
           */
           _T const * operator->() const
              { return &(_node->getItem()); }

          /*! \brief arrow operator
           */
           _T* operator->()
              { return &(_node->getItem()); }

          /*! \brief iterator compatation
           *  \param it Another const_iterator
           */
           bool operator==( const_iterator const &it ) const
              { return _node == it._node; }

          /*! \brief iterator compatation
           *  \param it Another const_iterator
           */
           bool operator!=( const_iterator const &it ) const
              { return _node != it._node; }

          /*! \brief iterator compatation
           *  \param it iterator
           */
           bool operator==( iterator const &it ) const
              { return _node == it._node; }

          /*! \brief iterator compatation
           *  \param it iterator
           */
           bool operator!=( iterator const &it ) const
              { return _node != it._node; }

          /*! \brief Returns the number of references of the internal node
           */
           int getReferences() const
           {
              if (_node != _list->_end ) {
                 return _node->getRefs();
              }
              return 0;
           }

          /*! \brief Increases the number of references of the internal node
           */
           void addReference()
           {
              if ( _node != _list->_end ) {
                 _node->increaseRefs();
              }
           }

          /*! \brief Decreases the number of references of the internal node
           */
           void deleteReference()
           {
              if ( _node != _list._end ) {
                 _node->decreaseRefs();
              }
           }
      };

   public:
      typedef std::list<ListNode*> NodeList; /**< Standard list of nodes */

   private:
      NodeList _freeList; /**< Nodes removed from the list are stored here */

      // FIXME: eventually this will be static
      int _N; /**< Negative value used to invalidate nodes atomically */

      Lock _lock; /**< List's Lock */

     /*! \brief List assign operator (private)
      */
      const List & operator= ( const List &list );

     /*! \brief List copy constructor (private)
      */
      List( const List &list );

   protected:
      ListNode* _begin;
      ListNode* _end;
      size_t _size;

   public:
     /*! \brief Default constructor
      */
      List() : _freeList(), _N(INT_MIN), _lock(), _begin(NULL), _end(NULL), _size(0) {}

     /*! \brief List destructor
      */
      ~List();

     /*! \brief Get the first element in the list
      */
      _T& front();

     /*! \brief Push an element into the list and return a reference to it
      */
      _T& push_front( const _T& elem );

     /*! \brief Deletes the first element in the list
      */
      void pop_front();

     /*! \brief Returns true if t was found and erased
      */
      bool erase( const _T& t );

     /*! \brief Returns an iterator to the begin of the list
      */
      iterator begin();

     /*! \brief Returns an interator to the end of the list
      */
      iterator end();

     /*! \brief Returns the number of elements in the list
      */
      size_t size() const;

     /*! \brief Returns the Lock of this list
      */
      Lock& getLock();

#if 0
     /*! \brief Get exclusive access to the list
      */
      void lock ( );

     /*! \brief Release list's lock
      */
      void unlock ( );
#endif
};

}
#endif

