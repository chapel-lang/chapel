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

#ifndef _NANOS_COPY_DESCRIPTOR_DECL
#define _NANOS_COPY_DESCRIPTOR_DECL

#include "compatibility.hpp"

namespace nanos {

   /*! \breif Class representing a copy used to synchronize the device with the cache */
   class CopyDescriptor {
      private:
         uint64_t _tag;
         unsigned int _dirVersion;
      public:
        /*! \brief Default constructor
         */
         CopyDescriptor( uint64_t tag, unsigned int dirVersion = 0 ): _tag(tag), _dirVersion(dirVersion) {}

        /*! \brief Copy constructor
         *  \param Another CopyDescriptor
         */
         CopyDescriptor( const CopyDescriptor &cd ): _tag( cd._tag ), _dirVersion( cd._dirVersion ) {}

        /* \brief Destructor
         */
         ~CopyDescriptor() {}

        /* \brief Assign operator
         */
         CopyDescriptor& operator=( const CopyDescriptor &cd )
         {
            if ( this == &cd ) return *this;
            this->_tag = cd._tag;
            this->_dirVersion = cd._dirVersion;
            return *this;
         }

         uint64_t getTag() const;
         unsigned int getDirectoryVersion() const;
   };
}

#endif

