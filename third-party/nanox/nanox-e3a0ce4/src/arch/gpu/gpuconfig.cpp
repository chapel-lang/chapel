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

#include "gpuconfig.hpp"
#include "plugin.hpp"
// We need to include system.hpp (to use verbose0(msg)), as debug.hpp does not include it
#include "system.hpp"

#include <cuda_runtime.h>

namespace nanos {
namespace ext {

bool GPUConfig::_disableCUDA = false;
int  GPUConfig::_numGPUs = -1;
bool GPUConfig::_prefetch = false;
bool GPUConfig::_overlap = false;
bool GPUConfig::_overlapInputs = false;
bool GPUConfig::_overlapOutputs = false;
transfer_mode GPUConfig::_transferMode = NANOS_GPU_TRANSFER_NORMAL;
size_t GPUConfig::_maxGPUMemory = 0;
bool GPUConfig::_gpuWarmup = true;
bool GPUConfig::_initCublas = false;
void * GPUConfig::_gpusProperties = NULL;

void GPUConfig::prepare( Config& config )
{
   config.setOptionsSection( "GPU Arch", "GPU specific options" );

   // Enable / disable CUDA
   config.registerConfigOption( "disable-cuda", NEW Config::FlagOption( _disableCUDA ),
                                "Enable or disable the use of GPUs with CUDA (enabled by default)" );
   config.registerEnvOption( "disable-cuda", "NX_DISABLECUDA" );
   config.registerArgOption( "disable-cuda", "disable-cuda" );

   // Set #GPUs
   config.registerConfigOption ( "num-gpus", NEW Config::IntegerVar( _numGPUs ),
                                 "Defines the maximum number of GPUs to use (defaults to the available number of GPUs in the system)" );
   config.registerEnvOption ( "num-gpus", "NX_GPUS" );
   config.registerArgOption ( "num-gpus", "gpus" );

   // Enable / disable prefetching
   config.registerConfigOption( "gpu-prefetch", NEW Config::FlagOption( _prefetch ),
                                "Set whether data prefetching must be activated or not (disabled by default)" );
   config.registerEnvOption( "gpu-prefetch", "NX_GPUPREFETCH" );
   config.registerArgOption( "gpu-prefetch", "gpu-prefetch" );

   // Enable / disable overlapping
   config.registerConfigOption( "gpu-overlap", NEW Config::FlagOption( _overlap ),
                                "Set whether GPU computation should be overlapped with\n\
                                     all data transfers, whenever possible, or not (disabled by default)" );
   config.registerEnvOption( "gpu-overlap", "NX_GPUOVERLAP" );
   config.registerArgOption( "gpu-overlap", "gpu-overlap" );

   // Enable / disable overlapping of inputs
   config.registerConfigOption( "gpu-overlap-inputs", NEW Config::FlagOption( _overlapInputs ),
                                "Set whether GPU computation should be overlapped with\n\
                                     host --> device data transfers, whenever possible, or not (disabled by default)" );
   config.registerEnvOption( "gpu-overlap-inputs", "NX_GPUOVERLAP_INPUTS" );
   config.registerArgOption( "gpu-overlap-inputs", "gpu-overlap-inputs" );

   // Enable / disable overlapping of outputs
   config.registerConfigOption( "gpu-overlap-outputs", NEW Config::FlagOption( _overlapOutputs ),
                                "Set whether GPU computation should be overlapped with\n\
                                     device --> host data transfers, whenever possible, or not (disabled by default)" );
   config.registerEnvOption( "gpu-overlap-outputs", "NX_GPUOVERLAP_OUTPUTS" );
   config.registerArgOption( "gpu-overlap-outputs", "gpu-overlap-outputs" );

   // Set maximum GPU memory available for each GPU
   config.registerConfigOption ( "gpu-max-memory", NEW Config::SizeVar( _maxGPUMemory ),
                                 "Defines the maximum amount of GPU memory (in bytes) to use for each GPU (defaults to the total amount of shared memory that each GPU has)" );
   config.registerEnvOption ( "gpu-max-memory", "NX_GPUMAXMEM" );
   config.registerArgOption ( "gpu-max-memory", "gpu-max-memory" );

   // Enable / disable GPU warmup
   config.registerConfigOption( "gpu-warmup", NEW Config::FlagOption( _gpuWarmup ),
                                "Enable or disable warming up the GPU (enabled by default)" );
   config.registerEnvOption( "gpu-warmup", "NX_GPUWARMUP" );
   config.registerArgOption( "gpu-warmup", "gpu-warmup" );

   // Enable / disable CUBLAS initialization
   config.registerConfigOption( "gpu-cublas-init", NEW Config::FlagOption( _initCublas ),
                                "Enable or disable CUBLAS initialization (disabled by default)" );
   config.registerEnvOption( "gpu-cublas-init", "NX_GPUCUBLASINIT" );
   config.registerArgOption( "gpu-cublas-init", "gpu-cublas-init" );
}

void GPUConfig::apply()
{
   if ( _disableCUDA ) {
      _numGPUs = 0;
      _prefetch = false;
      _overlap = false;
      _overlapInputs = false;
      _overlapOutputs = false;
      _maxGPUMemory = 0;
      _gpuWarmup = false;
      _initCublas = false;
      _gpusProperties = NULL;
   } else {
      // Find out how many CUDA-capable GPUs the system has
      int totalCount, device, deviceCount = 0;

      cudaError_t cudaErr = cudaGetDeviceCount( &totalCount );
      if ( cudaErr != cudaSuccess ) {
         totalCount = 0;
      }

      // Keep the information of GPUs in GPUDD, in order to avoid a second call to
      // 'cudaGetDeviceProperties()' for each GPU device
      _gpusProperties = NEW cudaDeviceProp[totalCount];
      struct cudaDeviceProp * gpuProperties = ( cudaDeviceProp * ) _gpusProperties;

      // Machines with no GPUs can still report one emulation device
      for ( device = 0; device < totalCount; device++ ) {
         cudaGetDeviceProperties( &gpuProperties[deviceCount], device );
         if ( gpuProperties[deviceCount].major != 9999 ) {
            // 9999 means emulation only
            deviceCount++;
         }
      }

      //displayAllGPUsProperties();

      // Check if the user has set a different number of GPUs to use
      if ( _numGPUs >= 0 ) {
         _numGPUs = std::min( _numGPUs, deviceCount );
      } else
         _numGPUs = deviceCount;

      // Check overlappings
      _overlapInputs = _overlap ? true : _overlapInputs;
      _overlapOutputs = _overlap ? true : _overlapOutputs;

      if ( _overlapInputs || _overlapOutputs ) {
         _transferMode = NANOS_GPU_TRANSFER_ASYNC;
      }

      if ( _initCublas ) {
         verbose( "initializing CUBLAS Library" );
         if ( !PluginManager::load ( "gpu-cublas", 1 ) ) {
            warning ( "Couldn't initialize CUBLAS library at runtime startup" );
         }
      }
   }

   printConfiguration();
}

void GPUConfig::printConfiguration()
{
   verbose0( "--- GPUDD configuration ---" );
   verbose0( "  Number of GPU's: " << _numGPUs );
   verbose0( "  Prefetching: " << ( _prefetch ? "Enabled" : "Disabled" ) );
   verbose0( "  Overlapping: " << ( _overlap ? "Enabled" : "Disabled" ) );
   verbose0( "  Overlapping inputs: " << ( _overlapInputs ? "Enabled" : "Disabled" ) );
   verbose0( "  Overlapping outputs: " << ( _overlapOutputs ? "Enabled" : "Disabled" ) );
   verbose0( "  Transfer mode: " << ( _transferMode == NANOS_GPU_TRANSFER_NORMAL ? "Sync" : "Async" ) );
   if ( _maxGPUMemory != 0 ) {
      verbose0( "  Limited memory: Enabled: " << _maxGPUMemory << " bytes" );
   }
   else {
      verbose0( "  Limited memory: Disabled" );
   }
   verbose0( "  GPU warm up: " << ( _gpuWarmup ? "Enabled" : "Disabled" ) );

   verbose0( "--- end of GPUDD configuration ---" );
}

void GPUConfig::getGPUsProperties( int device, void * deviceProps )
{
   void * props = &( ( cudaDeviceProp * ) _gpusProperties)[device];
   memcpy( deviceProps, props, sizeof( cudaDeviceProp ) );
}

}
}

