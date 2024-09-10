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
#include "chpl-topo.h"

#include <assert.h>

#ifndef __HIP_PLATFORM_AMD__
#define __HIP_PLATFORM_AMD__
#endif
#include <hip/hip_runtime.h>
#include <hip/hip_runtime_api.h>
#include <hip/hip_common.h>

// this is compiler-generated
extern const char* chpl_gpuBinary;

static int numAllDevices = -1;
static int numDevices = -1;
static int *deviceIDToIndex = NULL;
static hipDevice_t *indexToDeviceID = NULL;

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

static void switch_context(int dev_id) {
  ROCM_CALL(hipSetDevice(indexToDeviceID[dev_id]));
}

void chpl_gpu_impl_use_device(c_sublocid_t dev_id) {
  switch_context(dev_id);
}

static hipModule_t get_module(void) {
  hipDevice_t device;
  hipModule_t module;

  ROCM_CALL(hipGetDevice(&device));
  assert((((int) device) >= 0) && (((int) device) < numAllDevices));
  int index = deviceIDToIndex[(int)device];
  assert((index >= 0) && (index < numDevices));
  module = chpl_gpu_rocm_modules[index];
  return module;
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
    ROCM_CALL(hipMemcpyHtoD(ptr, (void*)&chpl_nodeID, glob_size));
  }
}


void chpl_gpu_impl_load_global(const char* global_name, void** ptr,
                               size_t* size) {
  hipModule_t module = get_module();

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

void* chpl_gpu_impl_load_function(const char* kernel_name) {
  hipFunction_t function;
  hipModule_t module = get_module();

  ROCM_CALL(hipModuleGetFunction(&function, module, kernel_name));
  assert(function);

  return (void*)function;
}

void chpl_gpu_impl_init(int* num_devices) {
  ROCM_CALL(hipInit(0));

  // Find all the GPUs (devices) on the machine then decide which we will
  // use. If there are co-locales then the GPUs are evenly divided among
  // them, otherwise we use them all.

  ROCM_CALL(hipGetDeviceCount(&numAllDevices));
  hipDevice_t *allDevices = chpl_malloc(sizeof(*allDevices) * numAllDevices);
  chpl_topo_pci_addr_t *allAddrs = chpl_malloc(sizeof(*allAddrs) * numAllDevices);
  // Find all the GPUs and get their PCI bus addresses.
  for (int i=0 ; i < numAllDevices; i++) {
#if ROCM_VERSION_MAJOR >= 6
    allDevices[i] = i;
#else
    ROCM_CALL(hipDeviceGet(&allDevices[i], i));
#endif
    int domain, bus, device;
#if ROCM_VERSION_MAJOR >= 5
    int rc = hipDeviceGetAttribute(&domain, hipDeviceAttributePciDomainID,
                                    allDevices[i]);
    if (rc == hipErrorInvalidValue) {
      // hipDeviceGetAttribute for hipDeviceAttributePciDomainID fails
      // on some (all?) platforms. Assume the domain is 0 and carry on.
      domain = 0;
    } else {
      ROCM_CALL(rc);
    }
#else
    // Earlier versions of ROCm don't support this attribute.
    domain = 0;
#endif
    ROCM_CALL(hipDeviceGetAttribute(&bus, hipDeviceAttributePciBusId,
                                    allDevices[i]));
    ROCM_CALL(hipDeviceGetAttribute(&device, hipDeviceAttributePciDeviceId,
                                    allDevices[i]));
    allAddrs[i].domain = (uint8_t) domain;
    allAddrs[i].bus = (uint8_t) bus;
    allAddrs[i].device = (uint8_t) device;
    allAddrs[i].function = 0;
  }

  // Call the topo module to determine which GPUs we should use.

  int numAddrs = numAllDevices;
  chpl_topo_pci_addr_t *addrs = chpl_malloc(sizeof(*addrs) * numAddrs);

  int rc = chpl_topo_selectMyDevices(allAddrs, addrs, &numAddrs);
  if (rc) {
    chpl_warning("unable to select GPUs for this locale, using them all",
                 0, 0);
    for (int i = 0; i < numAllDevices; i++) {
        addrs[i] = allAddrs[i];
    }
    numAddrs = numAllDevices;
  }

  // Allocate the GPU data structures. Note that the HIP API, specifically
  // hipGetDevice, returns the global device ID so we need deviceIDToIndex
  // to map from the global device ID to an array index.

  numDevices = numAddrs;
  chpl_gpu_rocm_modules = chpl_malloc(sizeof(hipModule_t)*numDevices);
  deviceClockRates = chpl_malloc(sizeof(int)*numDevices);
  indexToDeviceID = chpl_malloc(sizeof(int) * numDevices);
  deviceIDToIndex = chpl_malloc(sizeof(int) * numAllDevices);

  for (int i = 0; i < numDevices; i++) {
    chpl_gpu_rocm_modules[i] = NULL;
    indexToDeviceID[i] = -1;
  }

  for (int i = 0; i < numAllDevices; i++) {
    deviceIDToIndex[i] = -1;
  }

  // Go through the PCI bus addresses returned by chpl_topo_selectMyDevices
  // and find the corresponding GPUs. Initialize each GPU and its array
  // entries.

  int j = 0;
  for (int i = 0; i < numDevices; i++ ) {
    for (; j < numAllDevices; j++) {
      if (CHPL_TOPO_PCI_ADDR_EQUAL(&addrs[i], &allAddrs[j])) {
        hipDevice_t device = allDevices[j];
#if ROCM_VERSION_MAJOR >= 6
        ROCM_CALL(hipSetDevice(device));
        ROCM_CALL(hipSetDeviceFlags(hipDeviceScheduleBlockingSync))
#else
        hipCtx_t context;
        ROCM_CALL(hipDevicePrimaryCtxSetFlags(device, hipDeviceScheduleBlockingSync));
        ROCM_CALL(hipDevicePrimaryCtxRetain(&context, device));

        ROCM_CALL(hipSetDevice(device));
#endif
        hipModule_t module = chpl_gpu_load_module(chpl_gpuBinary);
        chpl_gpu_rocm_modules[i] = module;

        hipDeviceGetAttribute(&deviceClockRates[i],
                              hipDeviceAttributeClockRate, device);

        // map array indices (relative device numbers) to global device IDs
        indexToDeviceID[i] = device;
        deviceIDToIndex[device] = i;
        chpl_gpu_impl_set_globals(i, module);
        break;
      }
    }
  }
  chpl_free(allDevices);
  chpl_free(allAddrs);
  chpl_free(addrs);
  *num_devices = numDevices;
}

bool chpl_gpu_impl_is_device_ptr(const void* ptr) {
  if (!ptr) return false;
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

#if ROCM_VERSION_MAJOR >= 6
  // TODO: is this right?
  return res.type != hipMemoryTypeUnregistered;
#else
  return true;
#endif
}

bool chpl_gpu_impl_is_host_ptr(const void* ptr) {
  if (!ptr) return false;
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
#if ROCM_VERSION_MAJOR >= 6
    return res.type == hipMemoryTypeHost || res.type == hipMemoryTypeUnregistered;
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
    // see note in chpl_gpu_mem_free
    int64_t dev_id = c_sublocid_none;
    hipPointerAttribute_t res;
    ROCM_CALL(hipPointerGetAttributes(&res, memAlloc));
    switch_context(res.device);

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
  int id1 = indexToDeviceID[dev1];
  int id2 = indexToDeviceID[dev2];
  ROCM_CALL(hipDeviceCanAccessPeer(&p2p, id1, id2));
  return p2p != 0;
}

void chpl_gpu_impl_set_peer_access(int dev1, int dev2, bool enable) {
  int id1 = indexToDeviceID[dev1];
  int id2 = indexToDeviceID[dev2];
  ROCM_CALL(hipSetDevice(id1));
  if(enable) {
    ROCM_CALL(hipDeviceEnablePeerAccess(id2, 0));
  } else {
    ROCM_CALL(hipDeviceDisablePeerAccess(id2));
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
