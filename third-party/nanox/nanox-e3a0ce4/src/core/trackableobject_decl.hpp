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

#ifndef _NANOS_TRACKABLE_OBJECT_DECL_H
#define _NANOS_TRACKABLE_OBJECT_DECL_H
#include <stdlib.h>
#include <list>
#include "dependableobject_decl.hpp"
#include "commutationdepobj_decl.hpp"
#include "atomic_decl.hpp"

namespace nanos
{

  /*! \class TrackableObject
   *  \brief Object associated to an address with which different DependableObject operate
   */
   class TrackableObject
   {
      public:
         typedef std::list< DependableObject *> DependableObjectList; /**< Type list of DependableObject */
      private:
         void                  * _address; /**< Pointer to the dependency address */
         DependableObject      *_lastWriter; /**< Points to the last DependableObject registered as writer of the TrackableObject */
         DependableObjectList   _versionReaders; /**< List of readers of the last version of the object */
         Lock                   _readersLock; /**< Lock to provide exclusive access to the readers list */
         Lock                   _writerLock; /**< Lock internally the object for secure access to _lastWriter */
         CommutationDO         *_commDO; /**< Will be successor of all commutation tasks using this object untill a new reader/writer appears */
      public:

        /*! \brief TrackableObject default constructor
         *
         *  Creates a TrackableObject with the given address associated.
         */
         TrackableObject ( void * address = NULL )
            : _address(address), _lastWriter ( NULL ), _versionReaders(), _readersLock(), _writerLock(), _commDO(NULL) {}

        /*! \brief TrackableObject copy constructor
         *
         *  \param obj another TrackableObject
         */
         TrackableObject ( const TrackableObject &obj ) 
            :  _address ( obj._address ), _lastWriter ( obj._lastWriter ), _versionReaders(), _readersLock(), _writerLock(), _commDO(NULL) {}

        /*! \brief TrackableObject destructor
         */
         ~TrackableObject () {}

        /*! \brief TrackableObject assignment operator, can be self-assigned.
         *
         *  \param obj another TrackableObject
         */
         const TrackableObject & operator= ( const TrackableObject &obj );

        /*! \brief Obtain the address associated to the TrackableObject
         */
         void * getAddress ( );

        /*! \brief Returns true if the TrackableObject has a DependableObject as LastWriter
         */
         bool hasLastWriter ( );

        /*! \brief Get the last writer
         *  \sa DependableObject
         */
         DependableObject* getLastWriter ( );

        /*! \brief Set the last writer
         *  \sa DependableObject
         */
         void setLastWriter ( DependableObject &depObj );

        /*! \brief Delete the last writer if it matches the given one
         *  \param depObj DependableObject to compare with _lastWriter
         *  \sa DependableObject
         */
         void deleteLastWriter ( DependableObject &depObj );

        /*! \brief Get the list of readers
         *  \sa DependableObjectList
         */
         DependableObjectList & getReaders ( );

        /*! \brief Add a new reader
         *  \sa DependableObject
         */
         void setReader ( DependableObject &reader );

        /*! \brief Returns true if do is reader of the TrackableObject
         */
         bool hasReader ( DependableObject &depObj );

        /*! \brief Delete all readers from the object
         */ 
         void flushReaders ( );

        /*! \brief Deletes a reader from the object's list
         */
         void deleteReader ( DependableObject &reader );

        /*! \brief Whether the object has readers or not
         */
         bool hasReaders ();

        /*! \brief Returns the readers lock
         */
         Lock& getReadersLock();

        /*! \brief Returns the commutationDO if it exists
         */
         CommutationDO* getCommDO();

        /*! \brief Set the commutationDO
         *  \param commDO to set in this object
         */
         void setCommDO( CommutationDO *commDO );
   };

};

#endif
