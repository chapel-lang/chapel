/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

/*#define CHPL_GPU_PROFILE*/

static CUcontext *chpl_gpu_primary_ctx;
static int *deviceClockRates;

static CUmodule chpl_gpu_cuda_module;
static void* function_table[256];

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

  CHPL_GPU_DEBUG("Initializing GPU layer.\n");
  CHPL_GPU_DEBUG("  Memory allocation strategy for ---\n");
  #ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
    CHPL_GPU_DEBUG("    array data: device memory\n");
    CHPL_GPU_DEBUG("         other: page-locked host memory\n");
  #else
    CHPL_GPU_DEBUG("    array data: unified memory\n");
    CHPL_GPU_DEBUG("         other: unified memory\n");
  #endif

  // CUDA initialization
  CUDA_CALL(cuInit(0));

  CUDA_CALL(cuDeviceGetCount(&num_devices));

  chpl_gpu_primary_ctx = chpl_malloc(sizeof(CUcontext)*num_devices);
  deviceClockRates = chpl_malloc(sizeof(int)*num_devices);

  int i;
  for (i=0 ; i<num_devices ; i++) {
    CUdevice device;
    CUcontext context;

    CUDA_CALL(cuDeviceGet(&device, i));
    CUDA_CALL(cuDevicePrimaryCtxSetFlags(device, CU_CTX_SCHED_BLOCKING_SYNC));
    CUDA_CALL(cuDevicePrimaryCtxRetain(&context, device));
    cuDeviceGetAttribute(&deviceClockRates[i], CU_DEVICE_ATTRIBUTE_CLOCK_RATE, device);

    chpl_gpu_primary_ctx[i] = context;
  }

  for (i=0 ; i<256 ; i++) {
    function_table[i] = NULL;
  }
}

static bool chpl_gpu_device_alloc = false;

void chpl_gpu_impl_on_std_modules_finished_initializing(void) {
  // The standard module has some memory that we allocate when we  are "on" a
  // GPU sublocale when in fact we want to allocate it on the device. (As of
  // the writing of this comment this is in `helpSetupLocaleGPU` in
  // `LocaleModelHelpSetup`).
  //
  // Basically during the setup of the locale model we need to be "on" a given
  // sublocale when we instantiate the object for it (the expectation is that
  // the wide pointer for a sublocale appears to be on that sublocale),
  // but in practice we don't actually want the data for the GPU sublocale
  // object to be on the GPU).
  //
  // It's a bit of a hack but to handle this we start off setting
  // `chpl_gpu_device_alloc` to false indicating that we shouldn't actually
  // do any allocations on the device. Once the standard modules have finished
  // loading this callback function
  // (`chpl_gpu_impl_on_std_modules_finished_initializing`) gets called and we
  // flip the flag.
  chpl_gpu_device_alloc = true;
}

void chpl_gpu_get_device_count(int* into) {
  cudaGetDeviceCount(into);
}

bool chpl_gpu_impl_is_device_ptr(const void* ptr) {
  return chpl_gpu_common_is_device_ptr(ptr);
}

bool chpl_gpu_impl_is_host_ptr(const void* ptr) {
  unsigned int res;
  CUresult ret_val = cuPointerGetAttribute(&res,
                                           CU_POINTER_ATTRIBUTE_MEMORY_TYPE,
                                           (CUdeviceptr)ptr);

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

#ifdef CHPL_GPU_PROFILE
static long long get_time_in_ms() {
  struct timeval tv;

  gettimeofday(&tv, NULL);

  return (tv.tv_sec*1000000.0+tv.tv_usec);
}
#endif

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
#ifdef CHPL_GPU_PROFILE
  long long start_time = 0.0;

  long long context_time = 0.0;
  long long load_time = 0.0;
  long long prep_time = 0.0;
  long long kernel_time = 0.0;
  long long teardown_time = 0.0;

  start_time = get_time_in_ms();
#endif

  chpl_gpu_ensure_context();

#ifdef CHPL_GPU_PROFILE
  context_time = get_time_in_ms() - start_time;

  start_time = get_time_in_ms();
#endif


  if (chpl_gpu_cuda_module == NULL) {
    chpl_gpu_cuda_module = chpl_gpu_load_module(fatbinData);
  }
  void* function = chpl_gpu_load_function(chpl_gpu_cuda_module, name);

#ifdef CHPL_GPU_PROFILE
  load_time = get_time_in_ms() - start_time;

  start_time = get_time_in_ms();
#endif

  // TODO: this should use chpl_mem_alloc
  void*** kernel_params = chpl_malloc(nargs*sizeof(void**));

  assert(function);
  assert(kernel_params);

  CHPL_GPU_DEBUG("Creating kernel parameters\n");

  // Keep track of kernel parameters we dynamically allocate memory for so
  // later on we know what we need to free.
  bool* was_memory_dynamically_allocated_for_kernel_param =
    chpl_malloc(nargs*sizeof(bool));

  int i;
  for (i=0 ; i<nargs ; i++) {
    void* cur_arg = va_arg(args, void*);
    size_t cur_arg_size = va_arg(args, size_t);

    if (cur_arg_size > 0) {
      was_memory_dynamically_allocated_for_kernel_param[i] = true;

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
      was_memory_dynamically_allocated_for_kernel_param[i] = false;
      kernel_params[i] = cur_arg;
      CHPL_GPU_DEBUG("\tKernel parameter %d: %p\n",
                   i, kernel_params[i]);
    }
  }

#ifdef CHPL_GPU_PROFILE
  prep_time = get_time_in_ms() - start_time;

  start_time = get_time_in_ms();
#endif

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

#ifdef CHPL_GPU_PROFILE
  kernel_time = get_time_in_ms() - start_time;

  start_time = get_time_in_ms();
#endif

  // free GPU memory allocated for kernel parameters
  for (i=0 ; i<nargs ; i++) {
    if (was_memory_dynamically_allocated_for_kernel_param[i]) {
      chpl_gpu_mem_free(*kernel_params[i], ln, fn);
    }
  }

  // TODO: this should use chpl_mem_free
  chpl_free(kernel_params);


#ifdef CHPL_GPU_PROFILE
  teardown_time = get_time_in_ms() - start_time;

  printf("<%20s> Context: %Ld, Load: %Ld, Prep: %Ld, Kernel: %Ld, Teardown: %Ld\n",
         name, context_time, load_time, prep_time, kernel_time, teardown_time);
#endif
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

void* chpl_gpu_impl_memmove(void* dst, const void* src, size_t n) {
  #ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
  bool dst_on_host = chpl_gpu_impl_is_host_ptr(dst);
  bool src_on_host = chpl_gpu_impl_is_host_ptr(src);

  if (!dst_on_host && !src_on_host) {
    chpl_gpu_impl_copy_device_to_device(dst, src, n);
    return dst;
  }
  else if (!dst_on_host) {
    chpl_gpu_impl_copy_host_to_device(dst, src, n);
    return dst;
  }
  else if (!src_on_host) {
    chpl_gpu_impl_copy_device_to_host(dst, src, n);
    return dst;
  }
  else {
    assert(dst_on_host && src_on_host);
    return memmove(dst, src, n);
  }
  #else

  // for unified memory strategy we don't want to generate calls to copy
  // data from the device to host (since it can just be accessed directly)
  return memmove(dst, src, n);
  #endif
}

void chpl_gpu_impl_copy_device_to_host(void* dst, const void* src, size_t n) {
  assert(chpl_gpu_is_device_ptr(src));

  CUDA_CALL(cuMemcpyDtoH(dst, (CUdeviceptr)src, n));
}

void chpl_gpu_impl_copy_host_to_device(void* dst, const void* src, size_t n) {
  assert(chpl_gpu_is_device_ptr(dst));

  CUDA_CALL(cuMemcpyHtoD((CUdeviceptr)dst, src, n));
}

void chpl_gpu_impl_copy_device_to_device(void* dst, const void* src, size_t n) {
  assert(chpl_gpu_is_device_ptr(dst) && chpl_gpu_is_device_ptr(src));

  CUDA_CALL(cuMemcpyDtoD((CUdeviceptr)dst, (CUdeviceptr)src, n));
}

void* chpl_gpu_mem_array_alloc(size_t size, chpl_mem_descInt_t description,
                               int32_t lineno, int32_t filename) {
  chpl_gpu_ensure_context();

  CHPL_GPU_DEBUG("chpl_gpu_mem_array_alloc called. Size:%d file:%s line:%d\n", size,
               chpl_lookupFilename(filename), lineno);

  CUdeviceptr ptr = 0;
  if (size > 0) {
    chpl_memhook_malloc_pre(1, size, description, lineno, filename);
#ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
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

#ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
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
#ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
    if (chpl_gpu_impl_is_host_ptr(memAlloc)) {
      CUDA_CALL(cuMemFreeHost(memAlloc));
    }
    else {
#endif
    CUDA_CALL(cuMemFree((CUdeviceptr)memAlloc));
#ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
    }
#endif
  }
}

// This can be used for proper reallocation
size_t chpl_gpu_impl_get_alloc_size(void* ptr) {
  return chpl_gpu_common_get_alloc_size(ptr);
}

unsigned int chpl_gpu_device_clock_rate(int32_t devNum) {
  return (unsigned int)deviceClockRates[devNum];
}

#endif // HAS_GPU_LOCALE
