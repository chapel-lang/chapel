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

#include "gpuprocessor.hpp"
#include "debug.hpp"
#include "schedule.hpp"

#include "cuda_runtime.h"

using namespace nanos;
using namespace nanos::ext;

Atomic<int> GPUProcessor::_deviceSeed = 0;
size_t GPUProcessor::_memoryAlignment = 256;


GPUProcessor::GPUProcessor( int id, int gpuId ) : CachedAccelerator<GPUDevice>( id, &GPU ),
      _gpuDevice( _deviceSeed++ ), _gpuProcessorTransfers(), _allocator(), _pinnedMemory()
{
   _gpuProcessorInfo = NEW GPUProcessorInfo( gpuId );
}

GPUProcessor::~GPUProcessor()
{
   printStats();

   delete _gpuProcessorInfo;
}

void GPUProcessor::init ()
{
   // Each thread initializes its own GPUProcessor so that initialization
   // can be done in parallel

   struct cudaDeviceProp gpuProperties;
   GPUConfig::getGPUsProperties( _gpuDevice, ( void * ) &gpuProperties );
   //cudaGetDeviceProperties( &gpuProperties, _gpuDevice );

   // Check if the user has set the amount of memory to use (and the value is valid)
   // Otherwise, use 95% of the total GPU global memory
   size_t userDefinedMem = GPUConfig::getGPUMaxMemory();
   size_t maxMemoryAvailable = ( size_t ) ( gpuProperties.totalGlobalMem * 0.95 );

   if ( userDefinedMem > 0 ) {
      if ( userDefinedMem > maxMemoryAvailable ) {
         warning( "Could not set memory size to " << userDefinedMem
               << " for GPU #" << _gpuDevice
               << " because maximum memory available is " << maxMemoryAvailable
               << " bytes. Using " << maxMemoryAvailable << " bytes" );
      }
      else {
         maxMemoryAvailable = userDefinedMem;
      }
   }

   bool inputStream = GPUConfig::isOverlappingInputsDefined();
   bool outputStream = GPUConfig::isOverlappingOutputsDefined();

   if ( !gpuProperties.deviceOverlap ) {
      // It does not support stream overlapping, disable this feature
      warning( "Device #" << _gpuDevice
            << " does not support computation and data transfer overlapping" );
      inputStream = false;
      outputStream = false;
   }
   _gpuProcessorInfo->initTransferStreams( inputStream, outputStream );

   GPUConfig::setOverlappingInputs( inputStream );
   GPUConfig::setOverlappingOutputs( outputStream );

   // We allocate the whole GPU memory
   // WARNING: GPUDevice::allocateWholeMemory() must be called first, as it may
   // modify maxMemoryAvailable, in the case of not being able to allocate as
   // much bytes as we have asked
   void * baseAddress = GPUDevice::allocateWholeMemory( maxMemoryAvailable );
   _allocator.init( ( uint64_t ) baseAddress, maxMemoryAvailable );
   setCacheSize( maxMemoryAvailable );
   _gpuProcessorInfo->setMaxMemoryAvailable( maxMemoryAvailable );

   // WARNING: initTransferStreams() can modify inputStream's and outputStream's
   // value, so call it first

   /*
   if ( inputStream ) {
      // Create a list of inputs that have been ordered to transfer but the copy is
      // still not completed
      delete _gpuProcessorTransfers._pendingCopiesIn;
      _gpuProcessorTransfers._pendingCopiesIn = NEW GPUMemoryTransferInAsyncList();
   }
   */

   if ( outputStream ) {
      // If we have a stream for outputs, create the list with asynchronous behaviour
      delete _gpuProcessorTransfers._pendingCopiesOut;
      _gpuProcessorTransfers._pendingCopiesOut = NEW GPUMemoryTransferOutAsyncList();
   }
}

void GPUProcessor::freeWholeMemory()
{
   GPUDevice::freeWholeMemory( ( void * ) _allocator.getBaseAddress() );
}

size_t GPUProcessor::getMaxMemoryAvailable ( int id )
{
   return _gpuProcessorInfo->getMaxMemoryAvailable();
}

WorkDescriptor & GPUProcessor::getWorkerWD () const
{
   SMPDD * dd = NEW SMPDD( ( SMPDD::work_fct )Scheduler::workerLoop );
   WD *wd = NEW WD( dd );
   return *wd;
}

WorkDescriptor & GPUProcessor::getMasterWD () const
{
   fatal("Attempting to create a GPU master thread");
}

BaseThread &GPUProcessor::createThread ( WorkDescriptor &helper )
{
   // In fact, the GPUThread will run on the CPU, so make sure it canRunIn( SMP )
   ensure( helper.canRunIn( SMP ), "Incompatible worker thread" );
   GPUThread &th = *NEW GPUThread( helper, this, _gpuDevice );

   return th;
}


