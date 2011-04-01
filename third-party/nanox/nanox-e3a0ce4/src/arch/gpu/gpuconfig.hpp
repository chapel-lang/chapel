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

#ifndef _NANOS_GPU_CFG
#define _NANOS_GPU_CFG

#include "config.hpp"

#if __CUDA_API_VERSION < 3020
#define CUDANODEVERR cudaErrorNoDevice
#else
#define CUDANODEVERR cudaErrorDevicesUnavailable
#endif


namespace nanos {
namespace ext
{

   typedef enum {
      NANOS_GPU_TRANSFER_NORMAL,         //!  Basic transfer mode with no overlap
      NANOS_GPU_TRANSFER_ASYNC,          //! -- A little bit better (gives bad results from time to time)
      NANOS_GPU_TRANSFER_PINNED_CUDA,    //! -- Slowdown of ~10x (gives always bad results)
      NANOS_GPU_TRANSFER_PINNED_OS,      //! -- Similar to NANOS_GPU_TRANSFER_NORMAL (correct results though mlock fails)
      NANOS_GPU_TRANSFER_WC              //! -- Same as NANOS_GPU_TRANSFER_PINNED_CUDA: Slowdown of ~10x (gives always bad results)
   } transfer_mode;

   class GPUPlugin;

   /*! Contains the general configuration for the GPU module */
   class GPUConfig
   {
      friend class GPUPlugin;
      private:
         static bool          _disableCUDA; //! Enable/disable all CUDA support
         static int           _numGPUs; //! Number of CUDA-capable GPUs
         static bool          _prefetch; //! Enable / disable data prefetching (set by the user)
         static bool          _overlap; //! Enable / disable computation and data transfer overlapping (set by the user)
         static bool          _overlapInputs;
         static bool          _overlapOutputs;
         static transfer_mode _transferMode; //! Data transfer's mode (synchronous / asynchronous, ...)
         static size_t        _maxGPUMemory; //! Maximum amount of memory for each GPU to use
         static bool          _gpuWarmup; //! Enable / disable driver warmup (during runtime startup)
         static bool          _initCublas; //! Init CUBLAS library during runtime startup
         static void *        _gpusProperties; //! Array of structs of cudaDeviceProp

         /*! Parses the GPU user options */
         static void prepare ( Config &config );
         /*! Applies the configuration options and retrieves the information of the GPUs of the system */
         static void apply ( void );

      public:
         GPUConfig() {}
         ~GPUConfig() {}

         /*! return the number of available GPUs */
         static int getGPUCount ( void ) { return _numGPUs; }

         static bool isPrefetchingDefined ( void ) { return _prefetch; }

         static bool isOverlappingInputsDefined ( void ) { return _overlapInputs; }

         static void setOverlappingInputs ( bool overlap ) { _overlapInputs = overlap; }

         static bool isOverlappingOutputsDefined ( void ) { return _overlapOutputs; }

         static void setOverlappingOutputs ( bool overlap ) { _overlapOutputs = overlap; }

         /* \brief get the transfer mode for GPU devices */
         static transfer_mode getTransferMode ( void ) { return _transferMode; }

         static size_t getGPUMaxMemory( void ) { return _maxGPUMemory; }

         static bool isGPUWarmupDefined ( void ) { return _gpuWarmup; }

         static void getGPUsProperties( int device, void * deviceProps );

         static void printConfiguration( void );

   };


   // Macro's to instrument the code and make it cleaner
#define NANOS_GPU_CREATE_IN_CUDA_RUNTIME_EVENT(x)   NANOS_INSTRUMENT( { \
      Instrumentation::Event e; \
		sys.getInstrumentation()->createBurstEvent( \
		      &e, \
		      sys.getInstrumentation()->getInstrumentationDictionary()->getEventKey( "in-cuda-runtime" ), \
		      x ); \
      )

#define NANOS_GPU_CLOSE_IN_CUDA_RUNTIME_EVENT       NANOS_INSTRUMENT( \
      sys.getInstrumentation()->closeBurstEvent( \
            &e, sys.getInstrumentation()->getInstrumentationDictionary()->getEventKey( "in-cuda-runtime" ) ); \
      } )

   typedef enum {
      NANOS_GPU_CUDA_MALLOC_EVENT,
      NANOS_GPU_CUDA_FREE_EVENT,
      NANOS_GPU_CUDA_MALLOC_HOST_EVENT,
      NANOS_GPU_CUDA_FREE_HOST_EVENT,
      NANOS_GPU_CUDA_MEMCOPY_TO_HOST_EVENT,
      NANOS_GPU_CUDA_MEMCOPY_TO_DEVICE_EVENT,
      NANOS_GPU_CUDA_MEMCOPY_ASYNC_TO_HOST_EVENT,
      NANOS_GPU_CUDA_MEMCOPY_ASYNC_TO_DEVICE_EVENT,
      NANOS_GPU_CUDA_INPUT_STREAM_SYNC_EVENT,
      NANOS_GPU_CUDA_OUTPUT_STREAM_SYNC_EVENT,
      NANOS_GPU_CUDA_KERNEL_STREAM_SYNC_EVENT,
      NANOS_GPU_CUDA_THREAD_SYNC_EVENT,
      NANOS_GPU_CUDA_SET_DEVICE_EVENT,
      NANOS_GPU_CUDA_GET_DEVICE_PROPS_EVENT,
      NANOS_GPU_CUDA_SET_DEVICE_FLAGS_EVENT,
      NANOS_GPU_CUDA_GET_LAST_ERROR_EVENT,
      NANOS_GPU_CUDA_GENERIC_EVENT
 } in_cuda_runtime_event_value;
}
}

#endif
