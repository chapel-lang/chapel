/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.  * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sys_basic.h"
#include "chplrt.h"
#include "chpl-mem.h"
#include "chpl-gpu.h"
#include "error.h"

#ifdef HAS_GPU_LOCALE

#include <cuda.h>
#include <cuda_runtime.h>
#include <assert.h>

// #define CHPL_GPU_DEBUG  // TODO: adjust Makefile for this

static void CHPL_GPU_LOG(const char *str, ...) {
#ifdef CHPL_GPU_DEBUG
  va_list args;
  va_start(args, str);
  vfprintf(stdout, str, args);
  va_end(args);
  fflush(stdout);
#endif
}

static void chpl_gpu_cuda_check(int err, const char* file, int line) {
  if(err != CUDA_SUCCESS) {
    const int msg_len = 80;
    char msg[msg_len];

    snprintf(msg, msg_len,
             "%s:%d: Error calling CUDA function. (Code: %d)",
             file, line, err);
    
    chpl_internal_error(msg);
  }
}

#define CUDA_CALL(call) do {\
  chpl_gpu_cuda_check((int)call, __FILE__, __LINE__);\
} while(0);

void chpl_gpu_init() { CUdevice    device;
  CUcontext   context;
  int         devCount;

  // CUDA initialization
  CUDA_CALL(cuInit(0));

  CUDA_CALL(cuDeviceGetCount(&devCount));

  CUDA_CALL(cuDeviceGet(&device, 0));

  // Create driver context
  CUDA_CALL(cuCtxCreate(&context, 0, device));
}

static void* chpl_gpu_getKernel(const char* fatbinFile, const char* kernelName) {
  CUmodule    cudaModule;
  CUfunction  function;

  //read in fatbin and store in buffer
  char * buffer = 0;
  long length;
  FILE * f = fopen (fatbinFile, "rb");

  if (f)
  {
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    buffer = (char* )chpl_malloc (length);
    if (buffer)
    {
      fread (buffer, 1, length, f);
    }
    fclose (f);
  }

  // Create module for object
  CUDA_CALL(cuModuleLoadData(&cudaModule, buffer));

  // Get kernel function
  CUDA_CALL(cuModuleGetFunction(&function, cudaModule, kernelName));

  return (void*)function;
}

static void chpl_gpu_check_device_ptr(void* ptr) {
  CUdeviceptr base;
  size_t size;
  CUDA_CALL(cuMemGetAddressRange(&base, &size, *((CUdeviceptr*)ptr)));

  assert(base);
  assert(size > 0);
}

void chpl_gpu_launch_kernel(const char* name, int grid_dim_x, int block_dim_x,
                            int nargs, ...) {
  va_list args;
  int i;

  void* function = chpl_gpu_getKernel("tmp/chpl__gpu.fatbin", name);
  // TODO: this should use chpl_mem_alloc
  void** kernel_params = chpl_malloc(nargs*sizeof(void*));

  assert(function);
  assert(kernel_params);

  CHPL_GPU_LOG("Creating kernel parameters\n");

  va_start(args, nargs);
  for (i=0 ; i<nargs ; i++) {
    kernel_params[i] = va_arg(args, void*);

    // TODO: we can remove this check after some point, or enable only if some
    // advanced debugging is enabled.
    chpl_gpu_check_device_ptr(kernel_params[i]);

    CHPL_GPU_LOG("\tKernel parameter %d: %p\n", i, kernel_params[i]);
  }
  va_end(args);

  CHPL_GPU_LOG("Calling gpu function named %s\n", name);

  CUDA_CALL(cuLaunchKernel((CUfunction)function,
                           grid_dim_x, 1, 1,  // grid dimensions
                           block_dim_x, 1, 1, // block dimensions
                           0,  // shared memory in bytes
                           0,  // stream ID
                           kernel_params,
                           NULL));  // extra options

  CHPL_GPU_LOG("Call returned %s\n", name);
  
  CUDA_CALL(cudaDeviceSynchronize());

  CHPL_GPU_LOG("Synchronization complete %s\n", name);

  // TODO: this should use chpl_mem_free
  chpl_free(kernel_params);
}

#endif // HAS_GPU_LOCALE

