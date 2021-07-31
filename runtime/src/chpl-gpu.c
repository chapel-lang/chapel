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


static void checkCudaErrorsHelp(int err, const char* fn_name) {
  if(err != CUDA_SUCCESS) {
    const int msg_len = 80;
    char msg[msg_len];

    snprintf(msg, msg_len,
             "Encountered error calling CUDA function %s. (Error code: %d)",
             fn_name, err);
    
    chpl_internal_error(msg);
  }
}

static void checkCudaErrors(CUresult err, const char* fn_name) {
  checkCudaErrorsHelp((int)err, fn_name);
}
static void checkCudaRTErrors(cudaError_t err, const char* fn_name) {
  checkCudaErrorsHelp((int)err, fn_name);
}

void chpl_gpu_init() {
  CUdevice    device;
  CUcontext   context;
  int         devCount;

  // CUDA initialization
  checkCudaErrors(cuInit(0), "cuInit");

  checkCudaErrors(cuDeviceGetCount(&devCount), "cuDeviceGetCount");

  checkCudaErrors(cuDeviceGet(&device, 0), "cuDeviceGet");

  // Create driver context
  checkCudaErrors(cuCtxCreate(&context, 0, device), "cuCtxCreate");
}

void* chpl_gpu_getKernel(const char* fatbinFile, const char* kernelName) {
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
  checkCudaErrors(cuModuleLoadData(&cudaModule, buffer), "cuModuleLoadData");

  // Get kernel function
  checkCudaErrors(cuModuleGetFunction(&function, cudaModule, kernelName),
                  "cuModuleGetFunction");

  return (void*)function;
}

static void chpl_gpu_check_device_ptr(void* ptr) {
  CUdeviceptr base;
  size_t size;
  checkCudaErrors(cuMemGetAddressRange(&base, &size, *((CUdeviceptr*)ptr)),
                  "cuMemGetAddressRange");

  assert(base);
  assert(size > 0);

  printf("Base for %p is %p with size %zu\n", ptr, (void*)base, size);
}

void chpl_gpu_launch_kernel(const char* name, int grid_dim_x, int block_dim_x,
                            int nargs, ...) {
  va_list args;
  int i;

  void* function = chpl_gpu_getKernel("tmp/chpl__gpu.fatbin", name);
  assert(function);
  // TODO: this should use chpl_mem_alloc
  void** kernel_params = chpl_malloc(nargs*sizeof(void*));
  assert(kernel_params);

  printf("Creating kernel parameters\n");
  fflush(stdout);

  va_start(args, nargs);
  for (i=0 ; i<nargs ; i++) {
    kernel_params[i] = va_arg(args, void*);
    chpl_gpu_check_device_ptr(kernel_params[i]);
    printf("Kernel parameter %d: %p\n", i, kernel_params[i]);
    fflush(stdout);
  }

  va_end(args);

  printf("Calling gpu function named %s\n", name);
  fflush(stdout);

  checkCudaErrors(cuLaunchKernel((CUfunction)function,
                                  grid_dim_x, 1, 1,  // grid dimensions
                                  block_dim_x, 1, 1, // block dimensions
                                  0,  // shared memory in bytes
                                  0,  // stream ID
                                  kernel_params,
                                  NULL),  // extra options
                  "cuLaunchKernel");

  printf("Call returned %s\n", name);
  fflush(stdout);
  
  checkCudaRTErrors(cudaDeviceSynchronize(), "cudaDeviceSynchronize");

  printf("Synchronization complete %s\n", name);
  fflush(stdout);

  /*chpl_free(kernel_params);*/
}

#endif // HAS_GPU_LOCALE

