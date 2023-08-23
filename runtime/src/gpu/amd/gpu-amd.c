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
#include "chpl-tasks.h"
#include "error.h"
#include "chplcgfns.h"
#include "chpl-env-gen.h"
#include "chpl-linefile-support.h"

#include <assert.h>

#define __HIP_PLATFORM_AMD__
#include <hip/hip_runtime.h>
#include <hip/hip_runtime_api.h>
#include <hip/hip_common.h>

static void chpl_gpu_rocm_check(int err, const char* file, int line) {
  if(err == hipErrorContextAlreadyInUse) { return; }
  if(err != hipSuccess) {
    const int msg_len = 256;
    char msg[msg_len];

    snprintf(msg, msg_len,
             "%s:%d: Error calling HIP function: %s (Code: %d)",
             file, line, hipGetErrorString((hipError_t)err), err);

    chpl_internal_error(msg);
  }
}

#define ROCM_CALL(call) do {\
  chpl_gpu_rocm_check((int)call, __FILE__, __LINE__);\
} while(0);

static inline
void* chpl_gpu_load_module(const char* fatbin_data) {
  hipModule_t rocm_module;

  ROCM_CALL(hipModuleLoadData(&rocm_module, fatbin_data));
  assert(rocm_module);

  return (void*)rocm_module;
}

static inline
void* chpl_gpu_load_function(hipModule_t rocm_module, const char* kernel_name) {
  hipFunction_t function;

  ROCM_CALL(hipModuleGetFunction(&function, rocm_module, kernel_name));
  assert(function);

  return (void*)function;
}

// this is compiler-generated
extern const char* chpl_gpuBinary;

// array indexed by device ID (we load the same module once for each GPU).
static hipModule_t *chpl_gpu_rocm_modules;

static int *deviceClockRates;


static void switch_context(int dev_id) {
  ROCM_CALL(hipSetDevice(dev_id));
}

void chpl_gpu_impl_use_device(c_sublocid_t dev_id) {
  switch_context(dev_id);
}

extern c_nodeid_t chpl_nodeID;

static void chpl_gpu_impl_set_globals(c_sublocid_t dev_id, hipModule_t module) {
  hipDeviceptr_t ptr;
  size_t glob_size;

  // Engin: The AMDGPU backend seems to optimize chpl_nodeID away when it is not
  // used.  So, we should not error out if we can't find its definition. We can
  // look into making sure that it remains in the module, which feels a bit
  // safer, admittedly. Note also that this is the only diff between nvidia and
  // amd implementations in terms of adjusting chpl_nodeID.
  int err = hipModuleGetGlobal(&ptr, &glob_size, module, "chpl_nodeID");
  if (err == hipErrorNotFound) {
    return;
  }
  ROCM_CALL(err);

  assert(glob_size == sizeof(c_nodeid_t));
  // chpl_gpu_impl_copy_host_to_device performs a validation using
  // hipPointerGetAttributes. However, apparently, that's not something you
  // should call on pointers returned from hipModuleGetGlobal. Just perform
  // the copy directly.
  ROCM_CALL(hipMemcpyHtoD(ptr, (void*)&chpl_nodeID, glob_size));
}


void chpl_gpu_impl_init(int* num_devices) {
  ROCM_CALL(hipInit(0));

  ROCM_CALL(hipGetDeviceCount(num_devices));

  const int loc_num_devices = *num_devices;
  chpl_gpu_rocm_modules = chpl_malloc(sizeof(hipModule_t)*loc_num_devices);
  deviceClockRates = chpl_malloc(sizeof(int)*loc_num_devices);

  int i;
  for (i=0 ; i<loc_num_devices ; i++) {
    hipDevice_t device;
    hipCtx_t context;

    ROCM_CALL(hipDeviceGet(&device, i));
    ROCM_CALL(hipDevicePrimaryCtxSetFlags(device, hipDeviceScheduleBlockingSync));
    ROCM_CALL(hipDevicePrimaryCtxRetain(&context, device));

    ROCM_CALL(hipSetDevice(device));
    hipModule_t module = chpl_gpu_load_module(chpl_gpuBinary);
    chpl_gpu_rocm_modules[i] = module;

    hipDeviceGetAttribute(&deviceClockRates[i], hipDeviceAttributeClockRate, device);

    chpl_gpu_impl_set_globals(i, module);
  }
}

bool chpl_gpu_impl_is_device_ptr(const void* ptr) {
  hipPointerAttribute_t res;
  hipError_t ret_val = hipPointerGetAttributes(&res, (hipDeviceptr_t)ptr);

  if (ret_val != hipSuccess) {
    if (ret_val == hipErrorInvalidValue ||
        ret_val == hipErrorNotInitialized ||
        ret_val == hipErrorDeinitialized) {
      return false;
    }
    else {
      ROCM_CALL(ret_val);
    }
  }

  return true;
}

bool chpl_gpu_impl_is_host_ptr(const void* ptr) {
  hipPointerAttribute_t res;
  hipError_t ret_val = hipPointerGetAttributes(&res, (hipDeviceptr_t)ptr);

  if (ret_val != hipSuccess) {
    if (ret_val == hipErrorInvalidValue ||
        ret_val == hipErrorNotInitialized ||
        ret_val == hipErrorDeinitialized) {
      return true;
    }
    else {
      ROCM_CALL(ret_val);
    }
  }
  else {
    return res.memoryType == hipMemoryTypeHost;
  }

  return true;
}

static void chpl_gpu_launch_kernel_help(int ln,
                                        int32_t fn,
                                        const char* name,
                                        int grd_dim_x,
                                        int grd_dim_y,
                                        int grd_dim_z,
                                        int blk_dim_x,
                                        int blk_dim_y,
                                        int blk_dim_z,
                                        int nargs,
                                        va_list args) {
  CHPL_GPU_START_TIMER(load_time);

  c_sublocid_t dev_id = chpl_task_getRequestedSubloc();
  hipDeviceptr_t rocm_module = chpl_gpu_rocm_modules[dev_id];
  void* function = chpl_gpu_load_function(rocm_module, name);

  CHPL_GPU_STOP_TIMER(load_time);
  CHPL_GPU_START_TIMER(prep_time);

  // TODO: this should use chpl_mem_alloc
  void*** kernel_params = chpl_malloc(nargs*sizeof(void**));

  assert(function);
  assert(kernel_params);

  CHPL_GPU_DEBUG("Creating kernel parameters\n");
  CHPL_GPU_DEBUG("\tgridDims=(%d, %d, %d), blockDims(%d, %d, %d)\n",
                 grd_dim_x, grd_dim_y, grd_dim_z,
                 blk_dim_x, blk_dim_y, blk_dim_z);

  // Keep track of kernel parameters we dynamically allocate memory for so
  // later on we know what we need to free.
  bool* was_memory_dynamically_allocated_for_kernel_param =
    chpl_malloc(nargs*sizeof(bool));

  for (int i=0 ; i<nargs ; i++) {
    void* cur_arg = va_arg(args, void*);
    size_t cur_arg_size = va_arg(args, size_t);

    if (cur_arg_size > 0) {
      was_memory_dynamically_allocated_for_kernel_param[i] = true;

      // TODO this allocation needs to use `chpl_mem_alloc` with a proper desc
      kernel_params[i] = chpl_malloc(1*sizeof(hipDeviceptr_t));

      *kernel_params[i] = chpl_gpu_mem_alloc(cur_arg_size,
                                             CHPL_RT_MD_GPU_KERNEL_ARG,
                                             ln, fn);

      chpl_gpu_impl_copy_host_to_device(*kernel_params[i], cur_arg,
                                        cur_arg_size);

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

  CHPL_GPU_STOP_TIMER(prep_time);
  CHPL_GPU_START_TIMER(kernel_time);

  ROCM_CALL(hipModuleLaunchKernel((hipFunction_t)function,
                           grd_dim_x, grd_dim_y, grd_dim_z,
                           blk_dim_x, blk_dim_y, blk_dim_z,
                           0,  // shared memory in bytes
                           0,  // stream ID
                           (void**)kernel_params,
                           NULL));  // extra options

  CHPL_GPU_DEBUG("cuLaunchKernel returned %s\n", name);

  ROCM_CALL(hipDeviceSynchronize());

  CHPL_GPU_DEBUG("Synchronization complete %s\n", name);
  CHPL_GPU_STOP_TIMER(kernel_time);
  CHPL_GPU_START_TIMER(teardown_time);

  // free GPU memory allocated for kernel parameters
  for (int i=0 ; i<nargs ; i++) {
    if (was_memory_dynamically_allocated_for_kernel_param[i]) {
      chpl_gpu_mem_free(*kernel_params[i], ln, fn);
    }
  }

  // TODO: this should use chpl_mem_free
  chpl_free(kernel_params);

  CHPL_GPU_STOP_TIMER(teardown_time);
  CHPL_GPU_PRINT_TIMERS("<%20s> Load: %Lf, "
                               "Prep: %Lf, "
                               "Kernel: %Lf, "
                               "Teardown: %Lf\n",
         name, load_time, prep_time, kernel_time, teardown_time);
}

inline void chpl_gpu_impl_launch_kernel(int ln, int32_t fn,
                                        const char* name,
                                        int grd_dim_x,
                                        int grd_dim_y,
                                        int grd_dim_z,
                                        int blk_dim_x,
                                        int blk_dim_y,
                                        int blk_dim_z,
                                        int nargs, va_list args) {
  chpl_gpu_launch_kernel_help(ln, fn,
                              name,
                              grd_dim_x, grd_dim_y, grd_dim_z,
                              blk_dim_x, blk_dim_y, blk_dim_z,
                              nargs, args);
}

inline void chpl_gpu_impl_launch_kernel_flat(int ln, int32_t fn,
                                             const char* name,
                                             int64_t num_threads,
                                             int blk_dim,
                                             int nargs,
                                             va_list args) {
  int grd_dim = (num_threads+blk_dim-1)/blk_dim;

  chpl_gpu_launch_kernel_help(ln, fn,
                              name,
                              grd_dim, 1, 1,
                              blk_dim, 1, 1,
                              nargs, args);
}

void* chpl_gpu_impl_memset(void* addr, const uint8_t val, size_t n) {
  assert(chpl_gpu_is_device_ptr(addr));

  ROCM_CALL(hipMemsetD8((hipDeviceptr_t)addr, (unsigned int)val, n));

  return addr;
}


void chpl_gpu_impl_copy_device_to_host(void* dst, const void* src, size_t n) {
  assert(chpl_gpu_is_device_ptr(src));

  ROCM_CALL(hipMemcpyDtoH(dst, (hipDeviceptr_t)src, n));
}

void chpl_gpu_impl_copy_host_to_device(void* dst, const void* src, size_t n) {
  assert(chpl_gpu_is_device_ptr(dst));

  ROCM_CALL(hipMemcpyHtoD((hipDeviceptr_t)dst, (void *)src, n));
}

void chpl_gpu_impl_copy_device_to_device(void* dst, const void* src, size_t n) {
  assert(chpl_gpu_is_device_ptr(dst) && chpl_gpu_is_device_ptr(src));

  ROCM_CALL(hipMemcpyDtoD((hipDeviceptr_t)dst, (hipDeviceptr_t)src, n));
}


void* chpl_gpu_impl_comm_async(void *dst, void *src, size_t n) {
  hipStream_t stream;
  hipStreamCreateWithFlags(&stream, hipStreamNonBlocking);
  hipMemcpyAsync((hipDeviceptr_t)dst, (hipDeviceptr_t)src, n, hipMemcpyDefault, stream);
  return stream;
}

void chpl_gpu_impl_comm_wait(void *stream) {
  hipStreamSynchronize((hipStream_t)stream);
  hipStreamDestroy((hipStream_t)stream);
}

void* chpl_gpu_impl_mem_array_alloc(size_t size) {
  assert(size>0);

  hipDeviceptr_t ptr = 0;

#ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
    ROCM_CALL(hipMalloc(&ptr, size));
#else
    ROCM_CALL(hipMallocManaged(&ptr, size, hipMemAttachGlobal));
#endif

  return (void*)ptr;
}

void* chpl_gpu_impl_mem_alloc(size_t size) {
#ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
  void* ptr = 0;
  ROCM_CALL(hipHostMalloc(&ptr, size, 0));
#else
  hipDeviceptr_t ptr = 0;
  ROCM_CALL(hipMallocManaged(&ptr, size, hipMemAttachGlobal));
#endif
  assert(ptr!=0);

  return (void*)ptr;
}

void chpl_gpu_impl_mem_free(void* memAlloc) {
  if (memAlloc != NULL) {
    assert(chpl_gpu_is_device_ptr(memAlloc));
#ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
    if (chpl_gpu_impl_is_host_ptr(memAlloc)) {
      ROCM_CALL(hipHostFree(memAlloc));
    }
    else {
#endif
    ROCM_CALL(hipFree((hipDeviceptr_t)memAlloc));
#ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
    }
#endif
  }
}

void chpl_gpu_impl_hostmem_register(void *memAlloc, size_t size) {
  // The ROCM driver uses DMA to transfer page-locked memory to the GPU; if
  // memory is not page-locked it must first be transferred into a page-locked
  // buffer, which degrades performance. So in the array_on_device mode we
  // choose to page-lock such memory even if it's on the host-side.
  #ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
  hipHostRegister(memAlloc, size, hipHostRegisterPortable);
  #endif
}

// This can be used for proper reallocation
size_t chpl_gpu_impl_get_alloc_size(void* ptr) {
  hipDeviceptr_t base;
  size_t size;
  ROCM_CALL(hipMemGetAddressRange(&base, &size, (hipDeviceptr_t)ptr));

  return size;
}

unsigned int chpl_gpu_device_clock_rate(int32_t devNum) {
  return (unsigned int)deviceClockRates[devNum];
}

bool chpl_gpu_impl_can_access_peer(int dev1, int dev2) {
  int p2p;
  ROCM_CALL(hipDeviceCanAccessPeer(&p2p, dev1, dev2));
  return p2p != 0;
}

void chpl_gpu_impl_set_peer_access(int dev1, int dev2, bool enable) {
  ROCM_CALL(hipSetDevice(dev1));
  if(enable) {
    ROCM_CALL(hipDeviceEnablePeerAccess(dev2, 0));
  } else {
    ROCM_CALL(hipDeviceDisablePeerAccess(dev2));
  }
}

#endif // HAS_GPU_LOCALE
