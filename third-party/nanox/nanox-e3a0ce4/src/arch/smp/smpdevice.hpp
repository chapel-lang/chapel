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

#ifndef _SMP_DEVICE
#define _SMP_DEVICE

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "workdescriptor_decl.hpp"
#include "processingelement_fwd.hpp"
#include "copydescriptor_decl.hpp"

namespace nanos
{

  /* \brief Device specialization for SMP architecture
   * provides functions to allocate and copy data in the device
   */
   class SMPDevice : public Device
   {
      public:
         /*! \brief SMPDevice constructor
          */
         SMPDevice ( const char *n ) : Device ( n ) {}

         /*! \brief SMPDevice copy constructor
          */
         SMPDevice ( const SMPDevice &arch ) : Device ( arch ) {}

         /*! \brief SMPDevice destructor
          */
         ~SMPDevice() {};

        /* \breif allocate size bytes in the device
         */
         static void * allocate( size_t size, ProcessingElement *pe )
         {
            return new char[size]; 
         }

        /* \brief free address
         */
         static void free( void *address, ProcessingElement *pe )
         {
            delete[] (char *) address;
         }

        /* \brief Reallocate and copy from address.
         */
         static void * realloc( void *address, size_t size, size_t old_size, ProcessingElement *pe )
         {
            return ::realloc( address, size );
         }

        /* \brief Copy from remoteSrc in the host to localDst in the device
         *        Returns true if the operation is synchronous
         */
         static bool copyIn( void *localDst, CopyDescriptor &remoteSrc, size_t size, ProcessingElement *pe )
         {
            memcpy( localDst, (void *)remoteSrc.getTag(), size );
            return true;
         }

        /* \brief Copy from localSrc in the device to remoteDst in the host
         *        Returns true if the operation is synchronous
         */
         static bool copyOut( CopyDescriptor &remoteDst, void *localSrc, size_t size, ProcessingElement *pe )
         {
            memcpy( (void *)remoteDst.getTag(), localSrc, size );
            return true;
         }

        /* \brief Copy localy in the device from src to dst
         */
         static void copyLocal( void *dst, void *src, size_t size, ProcessingElement *pe )
         {
            memcpy( dst, src, size );
         }

         static void syncTransfer( uint64_t hostAddress, ProcessingElement *pe)
         {
         }
   };
}

#endif
