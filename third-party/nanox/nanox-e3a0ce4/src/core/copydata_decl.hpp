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

#ifndef _NANOS_COPYDATA_DECL
#define _NANOS_COPYDATA_DECL

#include "nanos-int.h"

namespace nanos
{

  /*! \class CopyData
   *  \brief Contains information about Copies
   */
   class CopyData : public nanos_copy_data_internal_t
   {
      public:
        /*! \brief CopyData default constructor
         *  \param address Address of the CopyData's address 
         *  \param input Whether the CopyData is input or not 
         *  \param output Whether the CopyData is output or not
         */
         CopyData ( uint64_t addr = NULL, nanos_sharing_t nxSharing = NANOS_SHARED, bool input = false,
                    bool output = false, size_t storageSize = 0 );

        /*! \brief CopyData copy constructor
         *  \param obj another CopyData
         */
         CopyData ( const CopyData &cd );

        /*! \brief CopyData copy assignment operator, can be self-assigned.
         *  \param obj another CopyData
         */
         const CopyData & operator= ( const CopyData &cd );

        /*! \brief CopyData destructor
         */
         ~CopyData () {}
         
        /*! \brief Obtain the CopyData's address address
         */
         uint64_t getAddress() const;
         
        /*! \brief Set the CopyData's address address
         */
         void setAddress( uint64_t addr );
         
        /*! \brief returns true if it is an input CopyData
         */
         bool isInput() const;

        /*! \brief sets the CopyData input clause to b
         */
         void setInput( bool b );
         
        /*! \brief returns true if it is an output CopyData
         */
         bool isOutput() const;

        /*! \brief sets the CopyData output clause to b
         */
         void setOutput( bool b );
         
        /*! \brief  returns the CopyData's size
         */
         size_t getSize() const;

        /*! \brief Returns true if the data to copy is shared
         */
         bool isShared() const;

        /*! \brief Returns true if the data to copy is private
         */
         bool isPrivate() const;

         nanos_sharing_t getSharing() const;
   };
}

#endif
