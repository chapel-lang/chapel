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

#ifdef HAS_GPU_LOCALE

#include <cuda.h>
#include <cuda_runtime.h>
#include <assert.h>

#define 
static void checkCudaErrors(CUresult err) {
  if(err != CUDA_SUCCESS) {
    chpl_internal_error("Encountered error calling CUDA function");
  }
}

void chpl_gpu_init() {
  CUdevice    device;
  CUcontext   context;
  int         devCount;

  // CUDA initialization
  checkCudaErrors(cuInit(0));

  checkCudaErrors(cuDeviceGetCount(&devCount));

  checkCudaErrors(cuDeviceGet(&device, 0));

  // Create driver context
  checkCudaErrors(cuCtxCreate(&context, 0, device));
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
    buffer = (char* )malloc (length);
    if (buffer)
    {
      fread (buffer, 1, length, f);
    }
    fclose (f);
  }

  // Create module for object
  checkCudaErrors(cuModuleLoadData(&cudaModule, buffer));

  // Get kernel function
  checkCudaErrors(cuModuleGetFunction(&function, cudaModule, kernelName));

  return (void*)function;
}

#endif // HAS_GPU_LOCALE

