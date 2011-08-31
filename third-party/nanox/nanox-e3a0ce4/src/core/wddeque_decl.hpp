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

#ifndef _NANOS_LIB_WDDEQUE_DECL_H
#define _NANOS_LIB_WDDEQUE_DECL_H

#include <list>
#include "atomic_decl.hpp"
#include "debug.hpp"
#include "workdescriptor_decl.hpp"
#include "basethread_decl.hpp"

namespace nanos
{

   class SchedulePredicate
   {

      public:
         /*! \brief SchedulePredicate default constructor
          */
         SchedulePredicate () {}
         /*! \brief SchedulePredicate destructor
          */
         virtual ~SchedulePredicate() {}
         /*! \brief SchedulePredicate function call operator (pure virtual)
          */
         virtual bool operator() ( WorkDescriptor *wd ) = 0;
   };


   class WDDeque
   {
      private:
         typedef std::list<WorkDescriptor *> BaseContainer;

         BaseContainer     _dq;
         Lock              _lock;

      private:
         /*! \brief WDDeque copy constructor (private)
          */
         WDDeque ( const WDDeque & );
         /*! \brief WDDeque copy assignment operator (private)
          */
         const WDDeque & operator= ( const WDDeque & );
      public:
         /*! \brief WDDeque default constructor
          */
         WDDeque() : _dq(), _lock() {}
         /*! \brief WDDeque destructor
          */
         ~WDDeque() {}

         bool empty ( void ) const;
         size_t size() const;

         void push_front ( WorkDescriptor *wd );
         void push_back( WorkDescriptor *wd );

         template <typename Constraints>
         WorkDescriptor * popFrontWithConstraints ( BaseThread *thread );
         template <typename Constraints>
         WorkDescriptor * popBackWithConstraints ( BaseThread *thread );
         template <typename Constraints>
         bool removeWDWithConstraints( BaseThread *thread, WorkDescriptor *toRem, WorkDescriptor **next );

         WorkDescriptor * pop_front ( BaseThread *thread );
         WorkDescriptor * pop_back ( BaseThread *thread );

         bool removeWD( BaseThread *thread, WorkDescriptor *toRem, WorkDescriptor **next );

         static void increaseTasksInQueues( int tasks );
         static void decreaseTasksInQueues( int tasks );
   };

}

#endif

