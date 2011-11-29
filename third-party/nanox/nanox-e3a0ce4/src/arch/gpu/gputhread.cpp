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

#include "gputhread.hpp"
#include "gpuprocessor.hpp"
#include "instrumentationmodule_decl.hpp"
#include "schedule.hpp"
#include "system.hpp"

#include <cuda_runtime.h>

using namespace nanos;
using namespace nanos::ext;


void GPUThread::initializeDependent ()
{
   cudaError_t err;

   // Bind the thread to a GPU device
   NANOS_GPU_CREATE_IN_CUDA_RUNTIME_EVENT( NANOS_GPU_CUDA_SET_DEVICE_EVENT );
   err = cudaSetDevice( _gpuDevice );
   NANOS_GPU_CLOSE_IN_CUDA_RUNTIME_EVENT;
   if ( err != cudaSuccess )
      warning( "Couldn't set the GPU device for the thread: " << cudaGetErrorString( err ) );

   // Configure some GPU device flags
   if ( GPUConfig::getTransferMode() == NANOS_GPU_TRANSFER_PINNED_CUDA
         || GPUConfig::getTransferMode() == NANOS_GPU_TRANSFER_WC ) {
      NANOS_GPU_CREATE_IN_CUDA_RUNTIME_EVENT( NANOS_GPU_CUDA_SET_DEVICE_FLAGS_EVENT );
      err = cudaSetDeviceFlags( cudaDeviceMapHost | cudaDeviceBlockingSync );
      NANOS_GPU_CLOSE_IN_CUDA_RUNTIME_EVENT;
      if ( err != cudaSuccess )
         warning( "Couldn't set the GPU device flags: " << cudaGetErrorString( err ) );
   }
   else {
      NANOS_GPU_CREATE_IN_CUDA_RUNTIME_EVENT( NANOS_GPU_CUDA_SET_DEVICE_FLAGS_EVENT );
      err = cudaSetDeviceFlags( cudaDeviceScheduleSpin );
      NANOS_GPU_CLOSE_IN_CUDA_RUNTIME_EVENT;
      if ( err != cudaSuccess )
         warning( "Couldn't set the GPU device flags:" << cudaGetErrorString( err ) );
   }

   // Initialize GPUProcessor
   ( ( GPUProcessor * ) myThread->runningOn() )->init();

   // Warming up GPU's...
   if ( GPUConfig::isGPUWarmupDefined() ) {
      NANOS_GPU_CREATE_IN_CUDA_RUNTIME_EVENT( NANOS_GPU_CUDA_FREE_EVENT );
      cudaFree(0);
      NANOS_GPU_CLOSE_IN_CUDA_RUNTIME_EVENT;
   }

   // Reset CUDA errors that may have occurred inside the runtime initialization
   NANOS_GPU_CREATE_IN_CUDA_RUNTIME_EVENT( NANOS_GPU_CUDA_GET_LAST_ERROR_EVENT );
   err = cudaGetLastError();
   NANOS_GPU_CLOSE_IN_CUDA_RUNTIME_EVENT;
   if ( err != cudaSuccess )
      warning( "CUDA errors occurred during initialization:" << cudaGetErrorString( err ) );
}

void GPUThread::runDependent ()
{
   WD &work = getThreadWD();
   setCurrentWD( work );
   SMPDD &dd = ( SMPDD & ) work.activateDevice( SMP );
   dd.getWorkFct()( work.getData() );
   ( ( GPUProcessor * ) myThread->runningOn() )->getGPUProcessorInfo()->destroyTransferStreams();
}

void GPUThread::inlineWorkDependent ( WD &wd )
{
   GPUDD &dd = ( GPUDD & )wd.getActiveDevice();
   GPUProcessor &myGPU = * ( GPUProcessor * ) myThread->runningOn();

   if ( GPUConfig::isOverlappingInputsDefined() ) {
      // Wait for the input transfer stream to finish
      NANOS_GPU_CREATE_IN_CUDA_RUNTIME_EVENT( NANOS_GPU_CUDA_INPUT_STREAM_SYNC_EVENT );
      cudaStreamSynchronize( myGPU.getGPUProcessorInfo()->getInTransferStream() );
      NANOS_GPU_CLOSE_IN_CUDA_RUNTIME_EVENT;
      // Erase the wait input list and synchronize it with cache
      myGPU.getInTransferList()->clearMemoryTransfers();
   }

   // We wait for wd inputs, but as we have just waited for them, we could skip this step
   wd.start( WD::IsNotAUserLevelThread );

   if ( GPUConfig::isOverlappingOutputsDefined() ) {
      NANOS_GPU_CREATE_IN_CUDA_RUNTIME_EVENT( NANOS_GPU_CUDA_OUTPUT_STREAM_SYNC_EVENT );
      cudaStreamSynchronize( myGPU.getGPUProcessorInfo()->getOutTransferStream() );
      NANOS_GPU_CLOSE_IN_CUDA_RUNTIME_EVENT;
   }

   NANOS_INSTRUMENT ( InstrumentStateAndBurst inst1( "user-code", wd.getId(), NANOS_RUNNING ) );
   ( dd.getWorkFct() )( wd.getData() );

   if ( !GPUConfig::isOverlappingOutputsDefined() && !GPUConfig::isOverlappingInputsDefined() ) {
      // Wait for the GPU kernel to finish
      NANOS_GPU_CREATE_IN_CUDA_RUNTIME_EVENT( NANOS_GPU_CUDA_THREAD_SYNC_EVENT );
      cudaThreadSynchronize();
      NANOS_GPU_CLOSE_IN_CUDA_RUNTIME_EVENT;

      // Normally this instrumentation code is inserted by the compiler in the task outline.
      // But because the kernel call is asynchronous for GPUs we need to raise them manually here
      // when we know the kernel has really finished
      NANOS_INSTRUMENT ( raiseWDClosingEvents() );

      // Copy out results from tasks executed previously
      // Do it always, as another GPU may be waiting for results
      myGPU.getOutTransferList()->executeMemoryTransfers();
   }
   else {
      // Open a new substate instrumentation phase before copying out the results
      NANOS_INSTRUMENT ( InstrumentSubState inst2( NANOS_RUNTIME ) );
      myGPU.getOutTransferList()->executeMemoryTransfers();
   }

   if ( GPUConfig::isPrefetchingDefined() ) {
      NANOS_INSTRUMENT ( InstrumentSubState inst2( NANOS_RUNTIME ) );
      // Get next task in order to prefetch data to device memory
      WD *next = Scheduler::prefetch( ( nanos::BaseThread * ) this, wd );

      setNextWD( next );
      if ( next != 0 ) {
         next->init();
      }
   }

   if ( GPUConfig::isOverlappingOutputsDefined() || GPUConfig::isOverlappingInputsDefined() ) {
      // Wait for the GPU kernel to finish, if we have not waited before
      //cudaThreadSynchronize();
      NANOS_GPU_CREATE_IN_CUDA_RUNTIME_EVENT( NANOS_GPU_CUDA_KERNEL_STREAM_SYNC_EVENT );
      cudaStreamSynchronize( myGPU.getGPUProcessorInfo()->getKernelExecStream() );
      NANOS_GPU_CLOSE_IN_CUDA_RUNTIME_EVENT;

      // Normally this instrumentation code is inserted by the compiler in the task outline.
      // But because the kernel call is asynchronous for GPUs we need to raise them manually here
      // when we know the kernel has really finished
      NANOS_INSTRUMENT ( raiseWDClosingEvents() );
   }
}

void GPUThread::yield()
{
   ( ( GPUProcessor * ) runningOn() )->getInTransferList()->executeMemoryTransfers();
   ( ( GPUProcessor * ) runningOn() )->getOutTransferList()->executeMemoryTransfers();
}

void GPUThread::idle()
{
   ( ( GPUProcessor * ) runningOn() )->getInTransferList()->executeMemoryTransfers();
   ( ( GPUProcessor * ) runningOn() )->getOutTransferList()->removeMemoryTransfer();
}


void GPUThread::raiseWDClosingEvents ()
{
   if ( _wdClosingEvents ) {
      NANOS_INSTRUMENT(
            Instrumentation::Event e[2];
            sys.getInstrumentation()->closeBurstEvent( &e[0],
                  sys.getInstrumentation()->getInstrumentationDictionary()->getEventKey( "user-funct-name" ) );
            sys.getInstrumentation()->closeBurstEvent( &e[1],
                  sys.getInstrumentation()->getInstrumentationDictionary()->getEventKey( "user-funct-location" ) );

            sys.getInstrumentation()->addEventList( 2, e );
      );
      _wdClosingEvents = false;
   }
}
