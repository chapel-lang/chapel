/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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
#include "gpu/amd/rocm-utils.h"
#include "gpu/amd/rocm-version.h"


#include <assert.h>

#ifndef __HIP_PLATFORM_AMD__
#define __HIP_PLATFORM_AMD__
#endif
#include <hip/hip_runtime.h>
#include <hip/hip_runtime_api.h>
#include <hip/hip_common.h>

// this is compiler-generated
extern const char* chpl_gpuBinary;

// array indexed by device ID (we load the same module once for each GPU).
static hipModule_t *chpl_gpu_rocm_modules;

static int *deviceClockRates;



static inline
void* chpl_gpu_load_module(const char* fatbin_data) {
  hipModule_t rocm_module;

  ROCM_CALL(hipModuleLoadData(&rocm_module, fatbin_data));
  assert(rocm_module);

  return (void*)rocm_module;
}

void* chpl_gpu_impl_load_function(const char* kernel_name) {
  hipFunction_t function;
  hipDevice_t device;
  hipModule_t module;

  ROCM_CALL(hipGetDevice(&device));

  module = chpl_gpu_rocm_modules[(int)device];

  ROCM_CALL(hipModuleGetFunction(&function, module, kernel_name));
  assert(function);

  return (void*)function;
}


static void switch_context(int dev_id) {
  ROCM_CALL(hipSetDevice(dev_id));
}

void chpl_gpu_impl_use_device(c_sublocid_t dev_id) {
  switch_context(dev_id);
}

extern c_nodeid_t chpl_nodeID;

static void chpl_gpu_impl_set_globals(c_sublocid_t dev_id, hipModule_t module) {
  hipDeviceptr_t ptr = NULL;;
  size_t glob_size;

  chpl_gpu_impl_load_global("chpl_nodeID", (void**)&ptr, &glob_size);

  if (ptr) {
    assert(glob_size == sizeof(c_nodeid_t));

    // chpl_gpu_impl_copy_host_to_device performs a validation using
    // hipPointerGetAttributes. However, apparently, that's not something you
    // should call on pointers returned from hipModuleGetGlobal. Just perform
    // the copy directly.
    //
    // The validation only happens when built with assertions (commonly
    // enabled by CHPL_DEVELOPER), and chpl_gpu_impl_copy_host_to_device
    // only causes issues in that case.
    ROCM_CALL(hipMemcpyDtoD(ptr, (void*)&chpl_nodeID, glob_size));
  }
}


void chpl_gpu_impl_load_global(const char* global_name, void** ptr,
                               size_t* size) {

  hipDevice_t device;
  hipModule_t module;

  ROCM_CALL(hipGetDevice(&device));

  module = chpl_gpu_rocm_modules[(int)device];
  //
  // Engin: The AMDGPU backend seems to optimize globals away when they are not
  // used.  So, we should not error out if we can't find its definition. We can
  // look into making sure that it remains in the module, which feels a bit
  // safer, admittedly. Note also that this is the only diff between nvidia and
  // amd implementations in terms of adjusting chpl_nodeID.
  int err = hipModuleGetGlobal((hipDeviceptr_t*)ptr, size, module, global_name);
  if (err == hipErrorNotFound) {
    return;
  }
  ROCM_CALL(err);
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
  hipError_t ret_val = hipPointerGetAttributes(&res, ptr);

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

#if ROCM_VERSION_MAJOR >= 6
  // TODO: whats the correct expression here?
  // https://rocm.docs.amd.com/projects/HIP/en/docs-6.0.0/doxygen/html/group___memory.html#ga7c3e8663feebb7be9fd3a1e5139bcefc
  return res.type != hipMemoryTypeUnregistered;
   //res.type == hipMemoryTypeDevice ||
         //res.type == hipMemoryTypeArray ;
         //res.type == hipMemoryTypeUnified ||
         //res.type == hipMemoryTypeManaged;
#else
  return true;
#endif
}

bool chpl_gpu_impl_is_host_ptr(const void* ptr) {
  hipPointerAttribute_t res;
  hipError_t ret_val = hipPointerGetAttributes(&res, ptr);

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
#if ROCM_VERSION_MAJOR >= 6
    return res.type == hipMemoryTypeHost;
#else
    return res.memoryType == hipMemoryTypeHost;
#endif
  }

  return true;
}

void chpl_gpu_impl_launch_kernel(void* kernel,
                                 int grd_dim_x, int grd_dim_y, int grd_dim_z,
                                 int blk_dim_x, int blk_dim_y, int blk_dim_z,
                                 void* stream, void** kernel_params) {
  assert(kernel);

  ROCM_CALL(hipModuleLaunchKernel((hipFunction_t)kernel,
                                  grd_dim_x, grd_dim_y, grd_dim_z,
                                  blk_dim_x, blk_dim_y, blk_dim_z,
                                  0,  // shared memory in bytes
                                  stream,  // stream ID
                                  (void**)kernel_params,
                                  NULL));  // extra options
}

void* chpl_gpu_impl_memset(void* addr, const uint8_t val, size_t n,
                           void* stream) {
  assert(chpl_gpu_is_device_ptr(addr));

  ROCM_CALL(hipMemsetD8Async((hipDeviceptr_t)addr, (unsigned int)val, n,
                              (hipStream_t)stream));

  return addr;
}


void chpl_gpu_impl_copy_device_to_host(void* dst, const void* src, size_t n,
                                       void* stream) {
  assert(chpl_gpu_is_device_ptr(src));

  ROCM_CALL(hipMemcpyDtoHAsync(dst, (hipDeviceptr_t)src, n,
                               (hipStream_t)stream));
}

void chpl_gpu_impl_copy_host_to_device(void* dst, const void* src, size_t n,
                                       void* stream) {
  assert(chpl_gpu_is_device_ptr(dst));

  ROCM_CALL(hipMemcpyHtoDAsync((hipDeviceptr_t)dst, (void *)src, n,
                               (hipStream_t)stream));
}

void chpl_gpu_impl_copy_device_to_device(void* dst, const void* src, size_t n,
                                         void* stream) {
  assert(chpl_gpu_is_device_ptr(dst) && chpl_gpu_is_device_ptr(src));

  ROCM_CALL(hipMemcpyDtoDAsync((hipDeviceptr_t)dst, (hipDeviceptr_t)src, n,
                               (hipStream_t)stream));
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
    // hip doesn't have stream-ordered memory allocator (no hipMallocAsync)
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
    // hip doesn't have stream-ordered memory allocator (no hipFreeAsync)
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
  ROCM_CALL(hipHostRegister(memAlloc, size, hipHostRegisterPortable));
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

void chpl_gpu_impl_synchronize(void) {
  ROCM_CALL(hipDeviceSynchronize());
}

bool chpl_gpu_impl_stream_supported(void) {
  return true;
}

void* chpl_gpu_impl_stream_create(void) {
  hipStream_t stream;
  ROCM_CALL(hipStreamCreateWithFlags(&stream, hipStreamDefault));
  return (void*) stream;
}

void chpl_gpu_impl_stream_destroy(void* stream) {
  if (stream) {
    ROCM_CALL(hipStreamDestroy((hipStream_t)stream));
  }
}

bool chpl_gpu_impl_stream_ready(void* stream) {
  if (stream) {
    hipError_t res = hipStreamQuery(stream);
    if (res == hipErrorNotReady) {
      return false;
    }
    ROCM_CALL(res);
  }
  return true;
}

void chpl_gpu_impl_stream_synchronize(void* stream) {
  if (stream) {
    ROCM_CALL(hipStreamSynchronize(stream));
  }
}

bool chpl_gpu_impl_can_reduce(void) {
  return ROCM_VERSION_MAJOR>=5;
}

bool chpl_gpu_impl_can_sort(void){
  return chpl_gpu_impl_can_reduce();
}

void* chpl_gpu_impl_host_register(void* var, size_t size) {
  ROCM_CALL(hipHostRegister(var, size, hipHostRegisterPortable));
  void *dev_var;
  ROCM_CALL(hipHostGetDevicePointer(&dev_var, var, 0));
  return dev_var;
}

void chpl_gpu_impl_host_unregister(void* var) {
  ROCM_CALL(hipHostUnregister(var));
}


#endif // HAS_GPU_LOCALE
