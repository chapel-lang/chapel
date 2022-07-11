/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

#ifndef cuda_shared_h
#define cuda_shared_h

#include <cuda.h>
#include <cuda_runtime.h>

#include "cuda-utils.h"

static inline
void* chpl_gpu_getKernel(const char* fatbinData, const char* kernelName) {

  CUmodule    cudaModule;
  CUfunction  function;

  // Create module for object
  CUDA_CALL(cuModuleLoadData(&cudaModule, fatbinData));

  // Get kernel function
  CUDA_CALL(cuModuleGetFunction(&function, cudaModule, kernelName));

  return (void*)function;
}

// this is part of the interface (used by the module code as an extern)
static inline
bool chpl_gpu_common_is_device_ptr(void* ptr) {

  unsigned int res;

  // We call CUDA_CALL later, because we want to treat some error codes
  // separately
  CUresult ret_val = cuPointerGetAttribute(&res, CU_POINTER_ATTRIBUTE_MEMORY_TYPE,
                                           (CUdeviceptr)ptr);

  if (ret_val == CUDA_SUCCESS) {
    return res == CU_MEMORYTYPE_DEVICE || res == CU_MEMORYTYPE_UNIFIED;
  }
  else if (ret_val == CUDA_ERROR_INVALID_VALUE ||
           ret_val == CUDA_ERROR_NOT_INITIALIZED ||
           ret_val == CUDA_ERROR_DEINITIALIZED) {
    return false;  // this is a cpu pointer that CUDA doesn't even know about
  }

  // there must have been an error in calling the cuda function. report that.
  CUDA_CALL(ret_val);

  return false;
}
//
// This can be used for proper reallocation
static inline
size_t chpl_gpu_common_get_alloc_size(void* ptr) {
  CUdeviceptr base;
  size_t size;
  CUDA_CALL(cuMemGetAddressRange(&base, &size, (CUdeviceptr)ptr));

  return size;
}

#endif
