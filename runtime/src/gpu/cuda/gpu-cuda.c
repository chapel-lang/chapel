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

#ifdef HAS_GPU_LOCALE

#include "sys_basic.h"
#include "chplrt.h"
#include "chpl-mem.h"
#include "chpl-gpu.h"
#include "chpl-gpu-impl.h"
#include "chpl-linefile-support.h"
#include "chpl-tasks.h"
#include "error.h"
#include "chplcgfns.h"
#include "../common/cuda-utils.h"
#include "../common/cuda-shared.h"

#include <cuda.h>
#include <cuda_runtime.h>
#include <assert.h>
#include <stdbool.h>

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

  CHPL_GPU_DEBUG("Initing GPU layer. (Memtype: %s)\n",
#ifdef CHPL_GPU_MEM_UVA
      "uva"
#else
      "uvm"
#endif
      );

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

bool chpl_gpu_impl_is_device_ptr(const void* ptr) {
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

  int* gpu_alloc_map = chpl_malloc(nargs*sizeof(int));


  for (i=0 ; i<nargs ; i++) {
    void* cur_arg = va_arg(args, void*);
    size_t cur_arg_size = va_arg(args, size_t);

    if (cur_arg_size > 0) {
      gpu_alloc_map[i] = 1;

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
      gpu_alloc_map[i] = 0;
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

  // free GPU memory allocated for kernel parameters
  for (i=0 ; i<nargs ; i++) {
    if (gpu_alloc_map[i] == 1) {
      chpl_gpu_mem_free(*kernel_params[i], 0, 0);
    }
  }

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

static bool chpl_gpu_allocated_on_host(const void* memAlloc) {
  unsigned int res;
  CUresult ret_val = cuPointerGetAttribute(&res,
                                           CU_POINTER_ATTRIBUTE_MEMORY_TYPE,
                                           (CUdeviceptr)memAlloc);

  if (ret_val != CUDA_SUCCESS) {
    if (ret_val == CUDA_ERROR_INVALID_VALUE ||
        ret_val == CUDA_ERROR_NOT_INITIALIZED ||
        ret_val == CUDA_ERROR_DEINITIALIZED) {
      return true;
    }
    else {
      CUDA_CALL(ret_val);
    }
  }
  else {
    return res == CU_MEMORYTYPE_HOST;
  }

  return true;
}


void* chpl_gpu_memmove(void* dst, const void* src, size_t n) {
  if (!chpl_gpu_allocated_on_host(dst)) {
    assert(chpl_gpu_allocated_on_host(src) && "D to D not supported");
    chpl_gpu_copy_host_to_device(dst, src, n);
    return dst;
  }
  else if (!chpl_gpu_allocated_on_host(src)) {
    assert(chpl_gpu_allocated_on_host(dst) && "D to D not supported");
    chpl_gpu_copy_device_to_host(dst, src, n);
    return dst;
  }
  else {
    assert(chpl_gpu_allocated_on_host(src) &&
        chpl_gpu_allocated_on_host(dst));
    return memmove(dst, src, n);
  }
}

void chpl_gpu_impl_copy_device_to_host(void* dst, const void* src, size_t n) {
  assert(chpl_gpu_is_device_ptr(src));

  CUDA_CALL(cuMemcpyDtoH(dst, (CUdeviceptr)src, n));
}

void chpl_gpu_impl_copy_host_to_device(void* dst, const void* src, size_t n) {
  assert(chpl_gpu_is_device_ptr(dst));

  CUDA_CALL(cuMemcpyHtoD((CUdeviceptr)dst, src, n));
}

void chpl_gpu_impl_copy_device_to_device(void* dst, void* src, size_t n) {
  chpl_gpu_ensure_context();

  CUDA_CALL(cuMemcpyDtoD((CUdeviceptr)dst, (CUdeviceptr)src, n));
}

static bool chpl_gpu_device_alloc = false;

void chpl_gpu_enable_device_alloc() {
  chpl_gpu_device_alloc = true;
}

void chpl_gpu_disable_device_alloc() {
  chpl_gpu_device_alloc = false;
}

void* chpl_gpu_mem_array_alloc(size_t size, chpl_mem_descInt_t description,
                               int32_t lineno, int32_t filename) {
  chpl_gpu_ensure_context();

  CHPL_GPU_DEBUG("chpl_gpu_mem_array_alloc called. Size:%d file:%s line:%d\n", size,
               chpl_lookupFilename(filename), lineno);

  CUdeviceptr ptr = 0;
  if (size > 0) {
    chpl_memhook_malloc_pre(1, size, description, lineno, filename);
#ifdef CHPL_GPU_MEM_UVA
    if (chpl_gpu_device_alloc) {
      CUDA_CALL(cuMemAlloc(&ptr, size));
    }
    else {
      void* mem = chpl_mem_alloc(size, description, lineno, filename);
      CHPL_GPU_DEBUG("\tregistering %p\n", mem);
      CUDA_CALL(cuMemHostRegister(mem, size, CU_MEMHOSTREGISTER_PORTABLE));
      CUDA_CALL(cuMemHostGetDevicePointer(&ptr, mem, 0));
    }
#else
    CUDA_CALL(cuMemAllocManaged(&ptr, size, CU_MEM_ATTACH_GLOBAL));
#endif
    chpl_memhook_malloc_post((void*)ptr, 1, size, description, lineno, filename);

    CHPL_GPU_DEBUG("chpl_gpu_mem_array_alloc returning %p\n", (void*)ptr);
  }
  else {
    CHPL_GPU_DEBUG("chpl_gpu_mem_array_alloc returning NULL (size was 0)\n");
  }

  return (void*)ptr;
}


void* chpl_gpu_impl_mem_alloc(size_t size) {
  chpl_gpu_ensure_context();

#ifdef CHPL_GPU_MEM_UVA
  void* ptr = 0;
  CUDA_CALL(cuMemAllocHost(&ptr, size));
#else
  CUdeviceptr ptr = 0;
  CUDA_CALL(cuMemAllocManaged(&ptr, size, CU_MEM_ATTACH_GLOBAL));
#endif
  assert(ptr!=0);

  return (void*)ptr;
}

void chpl_gpu_impl_mem_free(void* memAlloc) {
  chpl_gpu_ensure_context();

  if (memAlloc != NULL) {
    assert(chpl_gpu_is_device_ptr(memAlloc));
#ifdef CHPL_GPU_MEM_UVA
    if (chpl_gpu_allocated_on_host(memAlloc)) {
      CUDA_CALL(cuMemFreeHost(memAlloc));
    }
    else {
#endif
    CUDA_CALL(cuMemFree((CUdeviceptr)memAlloc));
#ifdef CHPL_GPU_MEM_UVA
    }
#endif
  }
}

// This can be used for proper reallocation
size_t chpl_gpu_impl_get_alloc_size(void* ptr) {
  return chpl_gpu_common_get_alloc_size(ptr);
}


#endif // HAS_GPU_LOCALE

