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

#ifndef _NANOS_GPU_PROCESSOR_DECL
#define _NANOS_GPU_PROCESSOR_DECL

#include "cachedaccelerator.hpp"
#include "gputhread.hpp"
#include "gpuconfig.hpp"
#include "gpudevice_decl.hpp"
#include "gpumemorytransfer.hpp"
#include "malign.hpp"
#include "simpleallocator_decl.hpp"
#include "copydescriptor_decl.hpp"

#include <map>


namespace nanos {
namespace ext
{

   class GPUProcessor : public CachedAccelerator<GPUDevice>
   {
      public:
         class GPUProcessorInfo;

         class GPUProcessorStats
         {
            public:
               unsigned int   _bytesIn;
               unsigned int   _bytesOut;
         };

         class GPUProcessorTransfers
         {
            public:
               GPUMemoryTransferList * _pendingCopiesIn;
               GPUMemoryTransferList * _pendingCopiesOut;


               GPUProcessorTransfers()
               {
                  _pendingCopiesIn = NEW GPUMemoryTransferInAsyncList();
                  _pendingCopiesOut = NEW GPUMemoryTransferOutSyncList();
               }

               ~GPUProcessorTransfers() 
               {
                  delete _pendingCopiesIn;
                  delete _pendingCopiesOut;
               }
         };


      private:
         // Configuration variables
         static Atomic<int>      _deviceSeed; //! Number of GPU devices assigned to threads
         int                     _gpuDevice; //! Assigned GPU device Id
         static size_t           _memoryAlignment;
         GPUProcessorInfo *      _gpuProcessorInfo; //! Information related to the GPU device that represents
         GPUProcessorStats       _gpuProcessorStats; //! Statistics of data copied in and out to / from cache
         GPUProcessorTransfers   _gpuProcessorTransfers; //! Keep the list of pending memory transfers


         SimpleAllocator               _allocator;
         std::map< void *, uint64_t >  _pinnedMemory;

         //! Disable copy constructor and assignment operator
         GPUProcessor( const GPUProcessor &pe );
         const GPUProcessor & operator= ( const GPUProcessor &pe );

         size_t getMaxMemoryAvailable ( int id );

      public:
         //! Constructors
         GPUProcessor( int id, int gpuId );

         virtual ~GPUProcessor();

         void init();
         void freeWholeMemory();

         WD & getWorkerWD () const;
         WD & getMasterWD () const;
         BaseThread & createThread ( WorkDescriptor &wd );

         //! Capability query functions
         bool supportsUserLevelThreads () const { return false; }

#if 0
         // Memory space support
         void setCacheSize( size_t size );

         void waitInputDependent( uint64_t tag );

         void registerCacheAccessDependent( Directory& dir, uint64_t tag, size_t size, bool input, bool output );
         void unregisterCacheAccessDependent( Directory& dir, uint64_t tag, size_t size, bool output );
         void registerPrivateAccessDependent( Directory& dir, uint64_t tag, size_t size, bool input, bool output );
         void unregisterPrivateAccessDependent( Directory& dir, uint64_t tag, size_t size );
         void synchronize( CopyDescriptor &cd );
         void synchronize( std::list<CopyDescriptor> &cds );

         void* getAddressDependent( uint64_t tag );
         void copyToDependent( void *dst, uint64_t tag, size_t size );
#endif
         // Allocator interface
         void * allocate ( size_t size )
         {
            return _allocator.allocate( NANOS_ALIGNED_MEMORY_OFFSET( 0, size, _memoryAlignment ) );
         }

         void free( void * address )
         {
            _allocator.free( address );
         }

         //! Get information about the GPU that represents this object
         GPUProcessorInfo * getGPUProcessorInfo ()
         {
            return _gpuProcessorInfo;
         }

         uint64_t getPinnedAddress ( void * dAddress )
         {
            return _pinnedMemory[dAddress];
         }

         void setPinnedAddress ( void * dAddress, uint64_t pinned )
         {
            _pinnedMemory[dAddress] = pinned;
         }

         void removePinnedAddress ( void * dAddress )
         {
            _pinnedMemory.erase( dAddress );
         }

         void transferInput ( size_t size )
         {
            _gpuProcessorStats._bytesIn += ( unsigned int ) size;
         }

         void transferOutput ( size_t size )
         {
            _gpuProcessorStats._bytesOut += ( unsigned int ) size;
         }

         GPUMemoryTransferList * getInTransferList ()
         {
            return _gpuProcessorTransfers._pendingCopiesIn;
         }

         GPUMemoryTransferList * getOutTransferList ()
         {
            return _gpuProcessorTransfers._pendingCopiesOut;
         }

         void printStats ()
         {
            message("GPU " << _gpuDevice << " TRANSFER STATISTICS");
            message("Total input transfers: " << _gpuProcessorStats._bytesIn << " bytes");
            message("Total output transfers: " << _gpuProcessorStats._bytesOut << " bytes");
         }
   };

}
}

#endif
