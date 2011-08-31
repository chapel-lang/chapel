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

#ifndef _NANOS_PROCESSING_ELEMENT_DECL
#define _NANOS_PROCESSING_ELEMENT_DECL

#include "workdescriptor_decl.hpp"
#include <algorithm>
#include "functors_decl.hpp"
#include "basethread_fwd.hpp"
#include "schedule_fwd.hpp"

namespace nanos
{
   class ProcessingElement
   {
      private:
         typedef std::vector<BaseThread *>    ThreadList;
         int                                  _id;
         const Device *                       _device;
         ThreadList                           _threads;

      private:
         /*! \brief ProcessinElement default constructor
          */
         ProcessingElement ();
         /*! \brief ProcessinElement copy constructor (private)
          */
         ProcessingElement ( const ProcessingElement &pe );
         /*! \brief ProcessinElement copy assignment operator (private)
          */
         const ProcessingElement & operator= ( const ProcessingElement &pe );
      protected:
         virtual WorkDescriptor & getMasterWD () const = 0;
         virtual WorkDescriptor & getWorkerWD () const = 0;
      public:
         /*! \brief ProcessinElement constructor
          */
         ProcessingElement ( int newId, const Device *arch ) : _id ( newId ), _device ( arch ) {}

         /*! \brief ProcessinElement destructor
          */
         virtual ~ProcessingElement();

         /* get/put methods */
         int getId() const;

         const Device & getDeviceType () const;

         BaseThread & startThread ( WorkDescriptor &wd );
         virtual BaseThread & createThread ( WorkDescriptor &wd ) = 0;
         BaseThread & associateThisThread ( bool untieMain=true );

         BaseThread & startWorker ( );
         void stopAll();

         /* capabilitiy query functions */
         virtual bool supportsUserLevelThreads() const = 0;
         virtual bool hasSeparatedMemorySpace() const { return false; }
         virtual unsigned int getMemorySpaceId() const { return 0; }

         virtual void waitInputDependent( uint64_t tag ) {}

         /* Memory space suport */
         virtual void copyDataIn( WorkDescriptor& wd );
         virtual void copyDataOut( WorkDescriptor& wd );

         virtual void waitInputs( WorkDescriptor& wd );

         virtual void* getAddress( WorkDescriptor& wd, uint64_t tag, nanos_sharing_t sharing );
         virtual void copyTo( WorkDescriptor& wd, void *dst, uint64_t tag, nanos_sharing_t sharing, size_t size );
   };

   typedef class ProcessingElement PE;
   typedef PE * ( *peFactory ) ( int pid );
};

#endif
