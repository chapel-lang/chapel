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

#include "sys_basic.h"
#include "chplrt.h"
#include "chpl-linefile-support.h"
#include "chpl-mem.h"
#include "chpl-gpu.h"
#include "chpl-tasks.h"
#include "error.h"
#include "chplcgfns.h"

#ifdef HAS_GPU_LOCALE


#include <cuda.h>
#include <cuda_runtime.h>
#include <assert.h>

static void CHPL_GPU_LOG(const char *str, ...) {
  if (verbosity >= 2) {
    va_list args;
    va_start(args, str);
    vfprintf(stdout, str, args);
    va_end(args);
    fflush(stdout);
  }
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

CUcontext *chpl_gpu_primary_ctx;

void chpl_gpu_init() {
  int         num_devices;

  // CUDA initialization
  CUDA_CALL(cuInit(0));

  CUDA_CALL(cuDeviceGetCount(&num_devices));

  chpl_gpu_primary_ctx = chpl_malloc(sizeof(CUcontext)*num_devices);

  int i;
  for (i=0 ; i<num_devices ; i++) {
    CUdevice device;
    CUcontext context;

    CUDA_CALL(cuDeviceGet(&device, i));
    CUDA_CALL(cuDevicePrimaryCtxRetain(&context, device));
    CUDA_CALL(cuDevicePrimaryCtxSetFlags(device, CU_CTX_SCHED_BLOCKING_SYNC));

    chpl_gpu_primary_ctx[i] = context;
  }
}

static void chpl_gpu_ensure_context() {
  CUcontext next_context = chpl_gpu_primary_ctx[chpl_task_getRequestedSubloc()-1];

  if (!chpl_gpu_has_context()) {
    CUDA_CALL(cuCtxPushCurrent(next_context));
  }
  else {
    CUcontext cur_context = NULL;
    cuCtxGetCurrent(&cur_context);
    if (cur_context == NULL) {
      chpl_internal_error("Unexpected GPU context error");
    }

    if (cur_context != next_context) {
      CUcontext popped;
      CUDA_CALL(cuCtxPopCurrent(&popped));
      CUDA_CALL(cuCtxPushCurrent(next_context));
    }
  }
}

static void* chpl_gpu_getKernel(const char* fatbinData, const char* kernelName) {
  chpl_gpu_ensure_context();

  CUmodule    cudaModule;
  CUfunction  function;

  // Create module for object
  CUDA_CALL(cuModuleLoadData(&cudaModule, fatbinData));

  // Get kernel function
  CUDA_CALL(cuModuleGetFunction(&function, cudaModule, kernelName));

  return (void*)function;
}

bool chpl_gpu_is_device_ptr(void* ptr) {
  //chpl_gpu_ensure_context();

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

size_t chpl_gpu_get_alloc_size(void* ptr) {
  chpl_gpu_ensure_context();

  CUdeviceptr base;
  size_t size;
  CUDA_CALL(cuMemGetAddressRange(&base, &size, (CUdeviceptr)ptr));

  return size;
}

bool chpl_gpu_running_on_gpu_locale() {
  return chpl_task_getRequestedSubloc()>0;
}

static void chpl_gpu_launch_kernel_help(const char* fatbinData,
                                        const char* name,
                                        int grd_dim_x,
                                        int grd_dim_y,
                                        int grd_dim_z,
                                        int blk_dim_x,
                                        int blk_dim_y,
                                        int blk_dim_z,
                                        int nargs,
                                        va_list args) {
  chpl_gpu_ensure_context();

  CHPL_GPU_LOG("Kernel launcher called. (subloc %d)\n"
               "\tKernel: %s\n"
               "\tGrid: %d,%d,%d\n"
               "\tBlock: %d,%d,%d\n"
               "\tNumArgs: %d\n",
               chpl_task_getRequestedSubloc(),
               name,
               grd_dim_x, grd_dim_y, grd_dim_z,
               blk_dim_x, blk_dim_y, blk_dim_z,
               nargs);

  int i;
  void* function = chpl_gpu_getKernel(fatbinData, name);
  // TODO: this should use chpl_mem_alloc
  void*** kernel_params = chpl_malloc(nargs*sizeof(void**));

  assert(function);
  assert(kernel_params);

  CHPL_GPU_LOG("Creating kernel parameters\n");

  for (i=0 ; i<nargs ; i++) {
    void* cur_arg = va_arg(args, void*);
    size_t cur_arg_size = va_arg(args, size_t);

    if (cur_arg_size > 0) {
      // TODO this allocation needs to use `chpl_mem_alloc` with a proper desc
      kernel_params[i] = chpl_malloc(1*sizeof(CUdeviceptr));

      // TODO pass the location info to this function and use a proper mem
      // desc
      *kernel_params[i] = chpl_gpu_mem_alloc(cur_arg_size, 0, 0, 0);

      chpl_gpu_copy_host_to_device(*kernel_params[i], cur_arg, cur_arg_size);

      CHPL_GPU_LOG("\tKernel parameter %d: %p (device ptr)\n",
                   i, *kernel_params[i]);
    }
    else {
      kernel_params[i] = cur_arg;
      CHPL_GPU_LOG("\tKernel parameter %d: %p\n",
                   i, kernel_params[i]);
    }
  }

  CHPL_GPU_LOG("Calling gpu function named %s\n", name);

  CUDA_CALL(cuLaunchKernel((CUfunction)function,
                           grd_dim_x, grd_dim_y, grd_dim_z,
                           blk_dim_x, blk_dim_y, blk_dim_z,
                           0,  // shared memory in bytes
                           0,  // stream ID
                           (void**)kernel_params,
                           NULL));  // extra options

  CHPL_GPU_LOG("Call returned %s\n", name);

  CUDA_CALL(cudaDeviceSynchronize());

  CHPL_GPU_LOG("Synchronization complete %s\n", name);

  // TODO: this should use chpl_mem_free
  chpl_free(kernel_params);
}

void chpl_gpu_copy_device_to_host(void* dst, void* src, size_t n) {
  chpl_gpu_ensure_context();

  assert(chpl_gpu_is_device_ptr(src));

  CHPL_GPU_LOG("Copying %zu bytes from device to host\n", n);

  CUDA_CALL(cuMemcpyDtoH(dst, (CUdeviceptr)src, n));
}

void chpl_gpu_copy_host_to_device(void* dst, void* src, size_t n) {
  chpl_gpu_ensure_context();

  assert(chpl_gpu_is_device_ptr(dst));

  CHPL_GPU_LOG("Copying %zu bytes from host to device\n", n);

  CUDA_CALL(cuMemcpyHtoD((CUdeviceptr)dst, src, n));
}

void chpl_gpu_launch_kernel(const char* fatbinData, const char* name,
                            int grd_dim_x, int grd_dim_y, int grd_dim_z,
                            int blk_dim_x, int blk_dim_y, int blk_dim_z,
                            int nargs, ...) {
  va_list args;
  va_start(args, nargs);
  chpl_gpu_launch_kernel_help(fatbinData, name,
                              grd_dim_x, grd_dim_y, grd_dim_z,
                              blk_dim_x, blk_dim_y, blk_dim_z,
                              nargs, args);
  va_end(args);
}

void chpl_gpu_launch_kernel_flat(const char* fatbinData, const char* name,
                                 int num_threads, int blk_dim, int nargs, ...) {
  int grd_dim = (num_threads+blk_dim-1)/blk_dim;

  va_list args;
  va_start(args, nargs);
  chpl_gpu_launch_kernel_help(fatbinData, name,
                              grd_dim, 1, 1,
                              blk_dim, 1, 1,
                              nargs, args);
  va_end(args);
}

bool chpl_gpu_has_context() {
  CUcontext cuda_context = NULL;

  CUresult ret = cuCtxGetCurrent(&cuda_context);

  if (ret == CUDA_ERROR_NOT_INITIALIZED || ret == CUDA_ERROR_DEINITIALIZED) {
    return false;
  }
  else {
    return cuda_context != NULL;
  }
}

void* chpl_gpu_mem_alloc(size_t size, chpl_mem_descInt_t description,
                         int32_t lineno, int32_t filename) {
  chpl_gpu_ensure_context();

  CHPL_GPU_LOG("chpl_gpu_mem_alloc called. Size:%d file:%s line:%d\n", size,
               chpl_lookupFilename(filename), lineno);

  CUdeviceptr ptr = 0;
  if (size > 0) {
    CUDA_CALL(cuMemAllocManaged(&ptr, size, CU_MEM_ATTACH_GLOBAL));
    CHPL_GPU_LOG("chpl_gpu_mem_alloc returning %p\n", (void*)ptr);
  }
  else {
    CHPL_GPU_LOG("chpl_gpu_mem_alloc returning NULL (size was 0)\n");
  }


  return (void*)ptr;

}

void* chpl_gpu_mem_calloc(size_t number, size_t size,
                          chpl_mem_descInt_t description,
                          int32_t lineno, int32_t filename) {
  chpl_gpu_ensure_context();

  CHPL_GPU_LOG("chpl_gpu_mem_calloc called. Size:%d\n", size);

  CUdeviceptr ptr;
  CUDA_CALL(cuMemAllocManaged(&ptr, size, CU_MEM_ATTACH_GLOBAL));
  CUDA_CALL(cuMemsetD8(ptr, 0, size));
  return (void*)ptr;
}

void* chpl_gpu_mem_realloc(void* memAlloc, size_t size,
                           chpl_mem_descInt_t description,
                           int32_t lineno, int32_t filename) {
  chpl_gpu_ensure_context();

  CHPL_GPU_LOG("chpl_gpu_mem_realloc called. Size:%d\n", size);

  assert(chpl_gpu_is_device_ptr(memAlloc));

  size_t cur_size = chpl_gpu_get_alloc_size(memAlloc);

  if (size == cur_size) {
    return memAlloc;
  }

  // TODO we could probably do something smarter, especially for the case where
  // the new allocation size is smaller than the original allocation size.

  CUdeviceptr ptr;
  CUDA_CALL(cuMemAllocManaged(&ptr, size, CU_MEM_ATTACH_GLOBAL));
  CUDA_CALL(cuMemcpyDtoD(ptr, (CUdeviceptr)memAlloc, cur_size));
  CUDA_CALL(cuMemFree((CUdeviceptr)memAlloc));

  return (void*)ptr;
}

void* chpl_gpu_mem_memalign(size_t boundary, size_t size,
                            chpl_mem_descInt_t description,
                            int32_t lineno, int32_t filename) {
  chpl_gpu_ensure_context();

  CHPL_GPU_LOG("chpl_gpu_mem_memalign called. Size:%d\n", size);
  chpl_internal_error("Not ready to allocate aligned memory on GPU, yet.");

  // ENGIN: I don't know if it is possible to allocate memory with custom
  // alignment on GPU. It looks like GPUs typically have a default alignment
  // (512?) that cannot be changed. I don't think we'd need more than that
  // today, and if we want, we can play some pointer games to return something
  // with a larger alignment here.

  return NULL;
}

void chpl_gpu_mem_free(void* memAlloc, int32_t lineno, int32_t filename) {
  chpl_gpu_ensure_context();

  CHPL_GPU_LOG("chpl_gpu_mem_free called. Ptr:%p file:%s line:%d\n", memAlloc,
               chpl_lookupFilename(filename), lineno);

  if (memAlloc != NULL) {
    assert(chpl_gpu_is_device_ptr(memAlloc));

    CUDA_CALL(cuMemFree((CUdeviceptr)memAlloc));
  }
}

#endif // HAS_GPU_LOCALE

