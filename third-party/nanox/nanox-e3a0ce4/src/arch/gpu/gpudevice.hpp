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

#ifndef _GPU_DEVICE
#define _GPU_DEVICE

#include "gpudevice_decl.hpp"
#include "gpuprocessor.hpp"

using namespace nanos;


GPUDevice::GPUDevice ( const char *n ) : Device ( n )
{
   getMemoryLockLimit();
}

GPUDevice::~GPUDevice() {}

void * GPUDevice::allocate( size_t size, ProcessingElement *pe )
{
   void * address = ( ( nanos::ext::GPUProcessor * ) pe )->allocate( size );
   if ( address == NULL ) return NULL;

   ( ( nanos::ext::GPUProcessor * ) pe )->setPinnedAddress( address, NULL );
   return address;
}

void GPUDevice::free( void *address, ProcessingElement *pe )
{
   // Check there are no pending copies to execute before we free the memory
   // (and if there are, execute them)
   ( ( nanos::ext::GPUProcessor * ) pe )->getOutTransferList()->checkAddressForMemoryTransfer( address );
   ( ( nanos::ext::GPUProcessor * ) pe )->free( address );

   uint64_t pinned = ( ( nanos::ext::GPUProcessor * ) pe )->getPinnedAddress( address );
   if ( pinned != 0 ) {
      freeIntermediateBuffer( pinned, address, pe );
   }
}

bool GPUDevice::copyIn( void *localDst, CopyDescriptor &remoteSrc, size_t size, ProcessingElement *pe )
{
   return ( myThread->runningOn() == pe ) ?
         isMycopyIn( localDst, remoteSrc, size, pe )
         : isNotMycopyIn( localDst, remoteSrc, size, pe );
}

bool GPUDevice::isNotMycopyIn( void *localDst, CopyDescriptor &remoteSrc, size_t size, ProcessingElement *pe )
{
   ( ( nanos::ext::GPUProcessor * ) pe )->getInTransferList()->addMemoryTransfer( remoteSrc, localDst, size );
   return false;
}

bool GPUDevice::isMycopyIn( void *localDst, CopyDescriptor &remoteSrc, size_t size, ProcessingElement *pe )
{
   // Copy from host memory to device memory
   // Check for synchronous or asynchronous mode
   if ( ( ( nanos::ext::GPUProcessor * ) pe )->getGPUProcessorInfo()->getInTransferStream() != 0 ) {
      copyInAsyncToBuffer( localDst, ( void * ) remoteSrc.getTag(), size );
      ( ( nanos::ext::GPUProcessor * ) pe )->getInTransferList()->addMemoryTransfer( remoteSrc );
      copyInAsyncToDevice( localDst, ( void * ) remoteSrc.getTag(), size );
      return false;
   }
   else {
      copyInSyncToDevice( localDst, ( void * ) remoteSrc.getTag(), size );
      return true;
   }
   return true;
}

bool GPUDevice::copyOut( CopyDescriptor &remoteDst, void *localSrc, size_t size, ProcessingElement *pe )
{
   return ( myThread->runningOn() == pe ) ?
         isMycopyOut( remoteDst, localSrc, size, pe )
         : isNotMycopyOut( remoteDst, localSrc, size, pe );
}

bool GPUDevice::isNotMycopyOut( CopyDescriptor &remoteDst, void *localSrc, size_t size, ProcessingElement *pe )
{
   ( ( nanos::ext::GPUProcessor * ) pe )->getOutTransferList()->addMemoryTransfer( remoteDst, localSrc, size );
   // Mark the copy as requested, because the thread invoking this function needs the data
   syncTransfer( remoteDst.getTag(), pe );
   return false;
}

bool GPUDevice::isMycopyOut( CopyDescriptor &remoteDst, void *localSrc, size_t size, ProcessingElement *pe )
{
   // Copy from device memory to host memory
   if ( ( ( nanos::ext::GPUProcessor * ) pe )->getGPUProcessorInfo()->getOutTransferStream() != 0 ) {
      ( ( nanos::ext::GPUProcessor * ) pe )->getOutTransferList()->addMemoryTransfer( remoteDst, localSrc, size );
      return false;
   }
   else {
      copyOutSyncToHost( ( void * ) remoteDst.getTag(), localSrc, size );
      return true;
   }
   return true;
}

void GPUDevice::syncTransfer( uint64_t hostAddress, ProcessingElement *pe)
{
   // syncTransfer() is used to ensure that somebody will update the data related to
   // 'hostAddress' of main memory at some time since we use copy back, this is always ensured

   // Anyway, we can help the system and tell that somebody is waiting for it
   ( ( nanos::ext::GPUProcessor * ) pe )->getOutTransferList()->requestTransfer( ( void * ) hostAddress );
}

void * GPUDevice::realloc( void * address, size_t size, size_t ceSize, ProcessingElement *pe )
{
   free( address, pe );
   return allocate( size, pe );
}

#endif
