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

#ifndef _NANOS_GPU_PROCESSOR
#define _NANOS_GPU_PROCESSOR

#include "gpuprocessor_decl.hpp"
#include "simpleallocator.hpp"

#include <cuda_runtime.h>

#include <iostream>


namespace nanos {
namespace ext
{

   class GPUProcessor::GPUProcessorInfo
   {
      private:
         // Device #
         unsigned int   _deviceId;

         // Memory
         size_t         _maxMemoryAvailable;

         // Transfers
         cudaStream_t   _inTransferStream;
         cudaStream_t   _outTransferStream;
         cudaStream_t   _localTransferStream;
         cudaStream_t   _kernelExecStream;

      public:
         GPUProcessorInfo ( int device ) : _deviceId( device ), _maxMemoryAvailable( 0 ),
            _inTransferStream( 0 ), _outTransferStream( 0 ), _localTransferStream( 0 ), _kernelExecStream( 0 )
         {}

         ~GPUProcessorInfo () {}

         void initTransferStreams ( bool &inputStream, bool &outputStream )
         {
            if ( inputStream ) {
               // Initialize the CUDA streams used for input data transfers
               cudaError_t err = cudaStreamCreate( &_inTransferStream );
               if ( err != cudaSuccess ) {
                  // If an error occurred, disable stream overlapping
                  _inTransferStream = 0;
                  inputStream = false;
                  if ( err == CUDANODEVERR ) {
                     fatal( "Error while creating the CUDA input transfer stream: all CUDA-capable devices are busy or unavailable" );
                  }
                  warning( "Error while creating the CUDA input transfer stream: " << cudaGetErrorString( err ) );
               }
            }

            if ( outputStream ) {
               // Initialize the CUDA streams used for output data transfers
               cudaError_t err = cudaStreamCreate( &_outTransferStream );
               if ( err != cudaSuccess ) {
                  // If an error occurred, disable stream overlapping
                  _outTransferStream = 0;
                  outputStream = false;
                  if ( err == CUDANODEVERR ) {
                     fatal( "Error while creating the CUDA output transfer stream: all CUDA-capable devices are busy or unavailable" );
                  }
                  warning( "Error while creating the CUDA output transfer stream: " << cudaGetErrorString( err ) );
               }
            }

            if ( inputStream || outputStream ) {
               // Initialize the CUDA streams used for local data transfers and kernel execution
               cudaError_t err = cudaStreamCreate( &_localTransferStream );
               if ( err != cudaSuccess ) {
                  // If an error occurred, disable stream overlapping
                  _localTransferStream = 0;
                  if ( err == CUDANODEVERR ) {
                     fatal( "Error while creating the CUDA output transfer stream: all CUDA-capable devices are busy or unavailable" );
                  }
                  warning( "Error while creating the CUDA local transfer stream: " << cudaGetErrorString( err ) );
               }
               err = cudaStreamCreate( &_kernelExecStream );
               if ( err != cudaSuccess ) {
                  // If an error occurred, disable stream overlapping
                  _kernelExecStream = 0;
                  if ( err == CUDANODEVERR ) {
                     fatal( "Error while creating the CUDA output transfer stream: all CUDA-capable devices are busy or unavailable" );
                  }
                  warning( "Error while creating the CUDA kernel execution stream: " << cudaGetErrorString( err ) );
               }
            }
         }

         void destroyTransferStreams ()
         {
            if ( _inTransferStream ) {
               cudaError_t err = cudaStreamDestroy( _inTransferStream );
               if ( err != cudaSuccess ) {
                  warning( "Error while destroying the CUDA input transfer stream: " << cudaGetErrorString( err ) );
               }
            }

            if ( _outTransferStream ) {
               cudaError_t err = cudaStreamDestroy( _outTransferStream );
               if ( err != cudaSuccess ) {
                  warning( "Error while destroying the CUDA output transfer stream: " << cudaGetErrorString( err ) );
               }
            }

            if ( _localTransferStream ) {
               cudaError_t err = cudaStreamDestroy( _localTransferStream );
               if ( err != cudaSuccess ) {
                  warning( "Error while destroying the CUDA local transfer stream: " << cudaGetErrorString( err ) );
               }
            }

            if ( _kernelExecStream ) {
               cudaError_t err = cudaStreamDestroy( _kernelExecStream );
               if ( err != cudaSuccess ) {
                  warning( "Error while destroying the CUDA kernel execution stream: " << cudaGetErrorString( err ) );
               }
            }
         }

         size_t getMaxMemoryAvailable ()
         {
            return _maxMemoryAvailable;
         }

         void setMaxMemoryAvailable ( size_t maxMemory )
         {
            _maxMemoryAvailable = maxMemory;
         }

         cudaStream_t getInTransferStream ()
         {
            return _inTransferStream;
         }

         cudaStream_t getOutTransferStream ()
         {
            return _outTransferStream;
         }

         cudaStream_t getLocalTransferStream ()
         {
            return _localTransferStream;
         }

         cudaStream_t getKernelExecStream ()
         {
            return _kernelExecStream;
         }
   };
}
}

#endif
