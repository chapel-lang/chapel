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
#include "chpl-mem.h"
#include "chpl-gpu.h"
#include "chpl-gpu-impl.h"
#include "chpl-tasks.h"
#include "error.h"
#include "chplcgfns.h"
#include "gpu-cuda-target.h"
#include "../common/cuda-utils.h"
#include "../common/cuda-shared.h"

#ifdef HAS_GPU_LOCALE

#include <cuda.h>
#include <cuda_runtime.h>
#include <assert.h>

static CUcontext *chpl_gpu_primary_ctx;

static bool chpl_gpu_has_context() {
  CUcontext cuda_context = NULL;

  CUresult ret = cuCtxGetCurrent(&cuda_context);

  if (ret == CUDA_ERROR_NOT_INITIALIZED || ret == CUDA_ERROR_DEINITIALIZED) {
    return false;
  }
  else {
    return cuda_context != NULL;
  }
}

static void chpl_gpu_ensure_context() {
  CUcontext next_context = chpl_gpu_primary_ctx[chpl_task_getRequestedSubloc()];

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

void chpl_gpu_impl_init() {
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
    CUDA_CALL(cuDevicePrimaryCtxSetFlags(device, CU_CTX_SCHED_BLOCKING_SYNC));
    CUDA_CALL(cuDevicePrimaryCtxRetain(&context, device));

    chpl_gpu_primary_ctx[i] = context;
  }
}

bool chpl_gpu_impl_is_device_ptr(void* ptr) {
  return chpl_gpu_common_is_device_ptr(ptr);
}

static void chpl_gpu_launch_kernel_help(int ln,
                                        int32_t fn,
                                        const char* fatbinData,
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

  int i;
  void* function = chpl_gpu_getKernel(fatbinData, name);
  // TODO: this should use chpl_mem_alloc
  void*** kernel_params = chpl_malloc(nargs*sizeof(void**));

  assert(function);
  assert(kernel_params);

  CHPL_GPU_DEBUG("Creating kernel parameters\n");

  for (i=0 ; i<nargs ; i++) {
    void* cur_arg = va_arg(args, void*);
    size_t cur_arg_size = va_arg(args, size_t);

    if (cur_arg_size > 0) {
      // TODO this allocation needs to use `chpl_mem_alloc` with a proper desc
      kernel_params[i] = chpl_malloc(1*sizeof(CUdeviceptr));

      *kernel_params[i] = chpl_gpu_mem_alloc(cur_arg_size,
                                             CHPL_RT_MD_GPU_KERNEL_ARG,
                                             ln, fn);

      chpl_gpu_copy_host_to_device(*kernel_params[i], cur_arg, cur_arg_size);

      CHPL_GPU_DEBUG("\tKernel parameter %d: %p (device ptr)\n",
                   i, *kernel_params[i]);
    }
    else {
      kernel_params[i] = cur_arg;
      CHPL_GPU_DEBUG("\tKernel parameter %d: %p\n",
                   i, kernel_params[i]);
    }
  }

  CUDA_CALL(cuLaunchKernel((CUfunction)function,
                           grd_dim_x, grd_dim_y, grd_dim_z,
                           blk_dim_x, blk_dim_y, blk_dim_z,
                           0,  // shared memory in bytes
                           0,  // stream ID
                           (void**)kernel_params,
                           NULL));  // extra options

  CHPL_GPU_DEBUG("cuLaunchKernel returned %s\n", name);

  CUDA_CALL(cudaDeviceSynchronize());

  CHPL_GPU_DEBUG("Synchronization complete %s\n", name);

  // TODO: this should use chpl_mem_free
  chpl_free(kernel_params);
}

void chpl_gpu_impl_launch_kernel(int ln, int32_t fn,
                                 const char* fatbinData, const char* name,
                                 int grd_dim_x, int grd_dim_y, int grd_dim_z,
                                 int blk_dim_x, int blk_dim_y, int blk_dim_z,
                                 int nargs, va_list args) {
  chpl_gpu_launch_kernel_help(ln, fn,
                              fatbinData, name,
                              grd_dim_x, grd_dim_y, grd_dim_z,
                              blk_dim_x, blk_dim_y, blk_dim_z,
                              nargs, args);
}

void chpl_gpu_impl_launch_kernel_flat(int ln, int32_t fn,
                                      const char* fatbinData, const char* name,
                                      int num_threads, int blk_dim, int nargs,
                                      va_list args) {
  int grd_dim = (num_threads+blk_dim-1)/blk_dim;

  chpl_gpu_launch_kernel_help(ln, fn,
                              fatbinData, name,
                              grd_dim, 1, 1,
                              blk_dim, 1, 1,
                              nargs, args);
}

void chpl_gpu_impl_copy_device_to_host(void* dst, void* src, size_t n) {
  chpl_gpu_ensure_context();

  CUDA_CALL(cuMemcpyDtoH(dst, (CUdeviceptr)src, n));
}

void chpl_gpu_impl_copy_host_to_device(void* dst, void* src, size_t n) {
  chpl_gpu_ensure_context();

  CUDA_CALL(cuMemcpyHtoD((CUdeviceptr)dst, src, n));
}

void chpl_gpu_impl_copy_device_to_device(void* dst, void* src, size_t n) {
  chpl_gpu_ensure_context();

  CUDA_CALL(cuMemcpyDtoD((CUdeviceptr)dst, (CUdeviceptr)src, n));
}

void* chpl_gpu_impl_mem_alloc(size_t size) {
  chpl_gpu_ensure_context();

  CUdeviceptr ptr = 0;
  CUDA_CALL(cuMemAllocManaged(&ptr, size, CU_MEM_ATTACH_GLOBAL));
  assert(ptr!=0);

  return (void*)ptr;
}

void chpl_gpu_impl_mem_free(void* memAlloc) {
  chpl_gpu_ensure_context();

  if (memAlloc != NULL) {
    assert(chpl_gpu_is_device_ptr(memAlloc));
    CUDA_CALL(cuMemFree((CUdeviceptr)memAlloc));
  }
}

/*void* chpl_gpu_impl_mem_calloc(size_t number, size_t size) {*/
  /*chpl_gpu_ensure_context();*/

  /*CUdeviceptr ptr;*/
  /*CUDA_CALL(cuMemAllocManaged(&ptr, size, CU_MEM_ATTACH_GLOBAL));*/
  /*CUDA_CALL(cuMemsetD8(ptr, 0, size));*/
  /*return (void*)ptr;*/
/*}*/

/*void* chpl_gpu_mem_realloc(void* memAlloc, size_t size,*/
                           /*chpl_mem_descInt_t description,*/
                           /*int32_t lineno, int32_t filename) {*/
  /*chpl_gpu_ensure_context();*/

  /*CHPL_GPU_DEBUG("chpl_gpu_mem_realloc called. Size:%d\n", size);*/

  /*assert(chpl_gpu_is_device_ptr(memAlloc));*/

  /*size_t cur_size = chpl_gpu_get_alloc_size(memAlloc);*/

  /*if (size == cur_size) {*/
    /*return memAlloc;*/
  /*}*/

  /*// TODO we could probably do something smarter, especially for the case where*/
  /*// the new allocation size is smaller than the original allocation size.*/

  /*CUdeviceptr ptr;*/
  /*CUDA_CALL(cuMemAllocManaged(&ptr, size, CU_MEM_ATTACH_GLOBAL));*/
  /*CUDA_CALL(cuMemcpyDtoD(ptr, (CUdeviceptr)memAlloc, cur_size));*/
  /*CUDA_CALL(cuMemFree((CUdeviceptr)memAlloc));*/

  /*return (void*)ptr;*/
/*}*/

/*void* chpl_gpu_mem_memalign(size_t boundary, size_t size,*/
                            /*chpl_mem_descInt_t description,*/
                            /*int32_t lineno, int32_t filename) {*/
  /*chpl_gpu_ensure_context();*/

  /*CHPL_GPU_DEBUG("chpl_gpu_mem_memalign called. Size:%d\n", size);*/
  /*chpl_internal_error("Not ready to allocate aligned memory on GPU, yet.");*/

  /*// ENGIN: I don't know if it is possible to allocate memory with custom*/
  /*// alignment on GPU. It looks like GPUs typically have a default alignment*/
  /*// (512?) that cannot be changed. I don't think we'd need more than that*/
  /*// today, and if we want, we can play some pointer games to return something*/
  /*// with a larger alignment here.*/

  /*return NULL;*/
/*}*/

// This can be used for proper reallocation
size_t chpl_gpu_impl_get_alloc_size(void* ptr) {
  return chpl_gpu_common_get_alloc_size(ptr);
}


#endif // HAS_GPU_LOCALE

