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

#include "gpuutils.hpp"
#include "system.hpp"

#include <iostream>
#include <iomanip>
#include <string.h>

#include "cuda_runtime.h"

namespace nanos {
namespace ext
{

void displayProperties( cudaDeviceProp* pDeviceProp, int device )
{
   if( !pDeviceProp )
      return;

   std::cout << std::endl;
   std::cout << std::endl << "Device " << device << ": " << pDeviceProp->name << " ";
   std::cout << std::endl << std::setw(50) << std::left << "  Major revision number:" << pDeviceProp->major;
   std::cout << std::endl << std::setw(50) << "  Minor revision number:" << pDeviceProp->minor;
   std::cout << std::endl << std::setw(50) << "  Total amount of global memory:" << pDeviceProp->totalGlobalMem/1024 << " KB";
   std::cout << std::endl << std::setw(50) << "  Number of multiprocessors:" << pDeviceProp->multiProcessorCount;
   std::cout << std::endl << std::setw(50) << "  Total amount of constant memory:" << pDeviceProp->totalConstMem << " bytes";
   std::cout << std::endl << std::setw(50) << "  Total amount of shared memory per block:" << pDeviceProp->sharedMemPerBlock/1024 << " KB";
   std::cout << std::endl << std::setw(50) << "  Total number of registers available per block:" << pDeviceProp->regsPerBlock;
   std::cout << std::endl << std::setw(50) << "  Warp size:" << pDeviceProp->warpSize << " threads";
   std::cout << std::endl << std::setw(50) << "  Maximum number of threads per block:" << pDeviceProp->maxThreadsPerBlock;
   std::cout << std::endl << std::setw(50) << "  Maximum sizes of each dimension of a block:" << pDeviceProp->maxThreadsDim[0] << " x " << pDeviceProp->maxThreadsDim[1] << " x " << pDeviceProp->maxThreadsDim[2];
   std::cout << std::endl << std::setw(50) << "  Maximum sizes of each dimension of a grid:" << pDeviceProp->maxGridSize[0] << " x " << pDeviceProp->maxGridSize[1] << " x " << pDeviceProp->maxGridSize[2];
   std::cout << std::endl << std::setw(50) << "  Maximum memory pitch:" << pDeviceProp->memPitch << " bytes";
   std::cout << std::endl << std::setw(50) << "  Texture Alignment:" << pDeviceProp->textureAlignment << " bytes";
   std::cout << std::endl << std::setw(50) << "  Clock rate:" << pDeviceProp->clockRate << " KHz";
   std::cout << std::endl << std::setw(50) << "  Concurrent copy and execution:" << ( pDeviceProp->deviceOverlap ? "Yes" : "No" );
   std::cout << std::endl << std::setw(50) << "  Can map host memory:" << ( pDeviceProp->canMapHostMemory ? "Yes" : "No" );
   std::cout << std::endl << std::setw(50) << "  Device integrated with host memory system:" << ( pDeviceProp->integrated ? "Yes" : "No" );
   std::cout << std::endl << std::setw(50) << "  Kernel execution timeout enabled:";
   if ( pDeviceProp->kernelExecTimeoutEnabled ) {
      std::cout << "Yes : " << pDeviceProp->kernelExecTimeoutEnabled;
   } else {
      std::cout << "No";
   }
   std::cout << std::endl << std::setw(50) << "  Compute mode:";
   if ( pDeviceProp->computeMode == cudaComputeModeExclusive ) {
      std::cout << "Exclusive";
   } else if ( pDeviceProp->computeMode == cudaComputeModeProhibited ) {
      std::cout << "Prohibited";
   } else if ( pDeviceProp->computeMode == cudaComputeModeDefault ) {
      std::cout << "Default";
   } else {
      std::cout << "Not specified";
   }
}

void displayAllGPUsProperties( void )
{
   cudaDeviceProp deviceProp;
   int idx, deviceCount = 0;

   cudaError_t err = cudaGetDeviceCount( &deviceCount );

   if ( err != cudaSuccess ) {
      std::cerr << cudaGetErrorString( err ) << std::endl;
      return;
   }

   std::cout << "Total number of GPUs found: " << deviceCount;

   for ( idx = 0; idx < deviceCount; idx++ ) {
      memset( &deviceProp, 0, sizeof( deviceProp ) );
      err = cudaGetDeviceProperties( &deviceProp, idx );

      if ( err == cudaSuccess ) {
         displayProperties( &deviceProp, idx );
      }
      else {
         std::cerr << cudaGetErrorString( err ) << std::endl;
      }
   }

   std::cout << std::endl;
}

} // namespace ext
} // namespace nanos

