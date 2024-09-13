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

const int CHPL_GPU_ATTRIBUTE__MAX_THREADS_PER_BLOCK = hipDeviceAttributeMaxThreadsPerBlock;

/* To implement when I have access to an AMD machine:

const int CHPL_GPU_ATTRIBUTE__MAX_BLOCK_DIM_X = CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_X;
const int CHPL_GPU_ATTRIBUTE__MAX_BLOCK_DIM_Y = CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_Y;
const int CHPL_GPU_ATTRIBUTE__MAX_BLOCK_DIM_Z = CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_Z;
const int CHPL_GPU_ATTRIBUTE__MAX_GRID_DIM_X = CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_X;
const int CHPL_GPU_ATTRIBUTE__MAX_GRID_DIM_Y = CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_Y;
const int CHPL_GPU_ATTRIBUTE__MAX_GRID_DIM_Z = CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_Z;
const int CHPL_GPU_ATTRIBUTE__MAX_SHARED_MEMORY_PER_BLOCK = CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_BLOCK;
const int CHPL_GPU_ATTRIBUTE__TOTAL_CONSTANT_MEMORY = CU_DEVICE_ATTRIBUTE_TOTAL_CONSTANT_MEMORY;
const int CHPL_GPU_ATTRIBUTE__WARP_SIZE = CU_DEVICE_ATTRIBUTE_WARP_SIZE;
const int CHPL_GPU_ATTRIBUTE__MAX_PITCH = CU_DEVICE_ATTRIBUTE_MAX_PITCH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE1D_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE1D_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE1D_LINEAR_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE1D_LINEAR_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE1D_MIPMAPPED_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE1D_MIPMAPPED_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE2D_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE2D_HEIGHT = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_HEIGHT;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE2D_LINEAR_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_LINEAR_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE2D_LINEAR_HEIGHT = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_LINEAR_HEIGHT;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE2D_LINEAR_PITCH = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_LINEAR_PITCH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE2D_MIPMAPPED_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_MIPMAPPED_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE2D_MIPMAPPED_HEIGHT = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_MIPMAPPED_HEIGHT;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE3D_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE3D_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE3D_HEIGHT = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE3D_HEIGHT;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE3D_DEPTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE3D_DEPTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE3D_WIDTH_ALTERNATE = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE3D_WIDTH_ALTERNATE;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE3D_HEIGHT_ALTERNATE = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE3D_HEIGHT_ALTERNATE;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE3D_DEPTH_ALTERNATE = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE3D_DEPTH_ALTERNATE;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURECUBEMAP_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURECUBEMAP_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE1D_LAYERED_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE1D_LAYERED_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE1D_LAYERED_LAYERS = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE1D_LAYERED_LAYERS;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE2D_LAYERED_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_LAYERED_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE2D_LAYERED_HEIGHT = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_LAYERED_HEIGHT;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE2D_LAYERED_LAYERS = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_LAYERED_LAYERS;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURECUBEMAP_LAYERED_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURECUBEMAP_LAYERED_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURECUBEMAP_LAYERED_LAYERS = CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURECUBEMAP_LAYERED_LAYERS;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_SURFACE1D_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE1D_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_SURFACE2D_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE2D_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_SURFACE2D_HEIGHT = CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE2D_HEIGHT;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_SURFACE3D_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE3D_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_SURFACE3D_HEIGHT = CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE3D_HEIGHT;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_SURFACE3D_DEPTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE3D_DEPTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_SURFACE1D_LAYERED_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE1D_LAYERED_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_SURFACE1D_LAYERED_LAYERS = CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE1D_LAYERED_LAYERS;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_SURFACE2D_LAYERED_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE2D_LAYERED_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_SURFACE2D_LAYERED_HEIGHT = CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE2D_LAYERED_HEIGHT;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_SURFACE2D_LAYERED_LAYERS = CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE2D_LAYERED_LAYERS;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_SURFACECUBEMAP_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACECUBEMAP_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_SURFACECUBEMAP_LAYERED_WIDTH = CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACECUBEMAP_LAYERED_WIDTH;
const int CHPL_GPU_ATTRIBUTE__MAXIMUM_SURFACECUBEMAP_LAYERED_LAYERS = CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACECUBEMAP_LAYERED_LAYERS;
const int CHPL_GPU_ATTRIBUTE__MAX_REGISTERS_PER_BLOCK = CU_DEVICE_ATTRIBUTE_MAX_REGISTERS_PER_BLOCK;
const int CHPL_GPU_ATTRIBUTE__CLOCK_RATE = CU_DEVICE_ATTRIBUTE_CLOCK_RATE;
const int CHPL_GPU_ATTRIBUTE__TEXTURE_ALIGNMENT = CU_DEVICE_ATTRIBUTE_TEXTURE_ALIGNMENT;
const int CHPL_GPU_ATTRIBUTE__TEXTURE_PITCH_ALIGNMENT = CU_DEVICE_ATTRIBUTE_TEXTURE_PITCH_ALIGNMENT;
const int CHPL_GPU_ATTRIBUTE__GPU_OVERLAP = CU_DEVICE_ATTRIBUTE_GPU_OVERLAP;
const int CHPL_GPU_ATTRIBUTE__MULTIPROCESSOR_COUNT = CU_DEVICE_ATTRIBUTE_MULTIPROCESSOR_COUNT;
const int CHPL_GPU_ATTRIBUTE__KERNEL_EXEC_TIMEOUT = CU_DEVICE_ATTRIBUTE_KERNEL_EXEC_TIMEOUT;
const int CHPL_GPU_ATTRIBUTE__INTEGRATED = CU_DEVICE_ATTRIBUTE_INTEGRATED;
const int CHPL_GPU_ATTRIBUTE__CAN_MAP_HOST_MEMORY = CU_DEVICE_ATTRIBUTE_CAN_MAP_HOST_MEMORY;
const int CHPL_GPU_ATTRIBUTE__COMPUTE_MODE = CU_DEVICE_ATTRIBUTE_COMPUTE_MODE;
const int CHPL_GPU_ATTRIBUTE__COMPUTEMODE_DEFAULT;
const int CHPL_GPU_ATTRIBUTE__COMPUTEMODE_PROHIBITED;
const int CHPL_GPU_ATTRIBUTE__PROCESS = CU_COMPUTEMODE_EXCLUSIVE_PROCESS;
const int CHPL_GPU_ATTRIBUTE__CONCURRENT_KERNELS = CU_DEVICE_ATTRIBUTE_CONCURRENT_KERNELS;
const int CHPL_GPU_ATTRIBUTE__ECC_ENABLED = CU_DEVICE_ATTRIBUTE_ECC_ENABLED;
const int CHPL_GPU_ATTRIBUTE__PCI_BUS_ID = CU_DEVICE_ATTRIBUTE_PCI_BUS_ID;
const int CHPL_GPU_ATTRIBUTE__PCI_DEVICE_ID = CU_DEVICE_ATTRIBUTE_PCI_DEVICE_ID;
const int CHPL_GPU_ATTRIBUTE__PCI_DOMAIN_ID = CU_DEVICE_ATTRIBUTE_PCI_DOMAIN_ID;
const int CHPL_GPU_ATTRIBUTE__TCC_DRIVER = CU_DEVICE_ATTRIBUTE_TCC_DRIVER;
const int CHPL_GPU_ATTRIBUTE__MEMORY_CLOCK_RATE = CU_DEVICE_ATTRIBUTE_MEMORY_CLOCK_RATE;
const int CHPL_GPU_ATTRIBUTE__GLOBAL_MEMORY_BUS_WIDTH = CU_DEVICE_ATTRIBUTE_GLOBAL_MEMORY_BUS_WIDTH;
const int CHPL_GPU_ATTRIBUTE__L2_CACHE_SIZE = CU_DEVICE_ATTRIBUTE_L2_CACHE_SIZE;
const int CHPL_GPU_ATTRIBUTE__MAX_THREADS_PER_MULTIPROCESSOR = CU_DEVICE_ATTRIBUTE_MAX_THREADS_PER_MULTIPROCESSOR;
const int CHPL_GPU_ATTRIBUTE__UNIFIED_ADDRESSING = CU_DEVICE_ATTRIBUTE_UNIFIED_ADDRESSING;
const int CHPL_GPU_ATTRIBUTE__COMPUTE_CAPABILITY_MAJOR = CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR;
const int CHPL_GPU_ATTRIBUTE__COMPUTE_CAPABILITY_MINOR = CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR;
const int CHPL_GPU_ATTRIBUTE__GLOBAL_L1_CACHE_SUPPORTED = CU_DEVICE_ATTRIBUTE_GLOBAL_L1_CACHE_SUPPORTED;
const int CHPL_GPU_ATTRIBUTE__LOCAL_L1_CACHE_SUPPORTED = CU_DEVICE_ATTRIBUTE_LOCAL_L1_CACHE_SUPPORTED;
const int CHPL_GPU_ATTRIBUTE__MAX_SHARED_MEMORY_PER_MULTIPROCESSOR = CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_MULTIPROCESSOR;
const int CHPL_GPU_ATTRIBUTE__MAX_REGISTERS_PER_MULTIPROCESSOR = CU_DEVICE_ATTRIBUTE_MAX_REGISTERS_PER_MULTIPROCESSOR;
const int CHPL_GPU_ATTRIBUTE__MANAGED_MEMORY = CU_DEVICE_ATTRIBUTE_MANAGED_MEMORY;
const int CHPL_GPU_ATTRIBUTE__MULTI_GPU_BOARD = CU_DEVICE_ATTRIBUTE_MULTI_GPU_BOARD;
const int CHPL_GPU_ATTRIBUTE__MULTI_GPU_BOARD_GROUP_ID = CU_DEVICE_ATTRIBUTE_MULTI_GPU_BOARD_GROUP_ID;
const int CHPL_GPU_ATTRIBUTE__HOST_NATIVE_ATOMIC_SUPPORTED = CU_DEVICE_ATTRIBUTE_HOST_NATIVE_ATOMIC_SUPPORTED;
const int CHPL_GPU_ATTRIBUTE__SINGLE_TO_DOUBLE_PRECISION_PERF_RATIO = CU_DEVICE_ATTRIBUTE_SINGLE_TO_DOUBLE_PRECISION_PERF_RATIO;
const int CHPL_GPU_ATTRIBUTE__PAGEABLE_MEMORY_ACCESS = CU_DEVICE_ATTRIBUTE_PAGEABLE_MEMORY_ACCESS;
const int CHPL_GPU_ATTRIBUTE__CONCURRENT_MANAGED_ACCESS = CU_DEVICE_ATTRIBUTE_CONCURRENT_MANAGED_ACCESS;
const int CHPL_GPU_ATTRIBUTE__COMPUTE_PREEMPTION_SUPPORTED = CU_DEVICE_ATTRIBUTE_COMPUTE_PREEMPTION_SUPPORTED;
const int CHPL_GPU_ATTRIBUTE__CAN_USE_HOST_POINTER_FOR_REGISTERED_MEM = CU_DEVICE_ATTRIBUTE_CAN_USE_HOST_POINTER_FOR_REGISTERED_MEM;
const int CHPL_GPU_ATTRIBUTE__MAX_SHARED_MEMORY_PER_BLOCK_OPTIN = CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_BLOCK_OPTIN;
const int CHPL_GPU_ATTRIBUTE__PAGEABLE_MEMORY_ACCESS_USES_HOST_PAGE_TABLES = CU_DEVICE_ATTRIBUTE_PAGEABLE_MEMORY_ACCESS_USES_HOST_PAGE_TABLES;
const int CHPL_GPU_ATTRIBUTE__DIRECT_MANAGED_MEM_ACCESS_FROM_HOST = CU_DEVICE_ATTRIBUTE_DIRECT_MANAGED_MEM_ACCESS_FROM_HOST;
const int CHPL_GPU_ATTRIBUTE__VIRTUAL_MEMORY_MANAGEMENT_SUPPORTED = CU_DEVICE_ATTRIBUTE_VIRTUAL_MEMORY_MANAGEMENT_SUPPORTED;
const int CHPL_GPU_ATTRIBUTE__HANDLE_TYPE_POSIX_FILE_DESCRIPTOR_SUPPORTED = CU_DEVICE_ATTRIBUTE_HANDLE_TYPE_POSIX_FILE_DESCRIPTOR_SUPPORTED;
const int CHPL_GPU_ATTRIBUTE__HANDLE_TYPE_WIN32_HANDLE_SUPPORTED = CU_DEVICE_ATTRIBUTE_HANDLE_TYPE_WIN32_HANDLE_SUPPORTED;
const int CHPL_GPU_ATTRIBUTE__HANDLE_TYPE_WIN32_KMT_HANDLE_SUPPORTED = CU_DEVICE_ATTRIBUTE_HANDLE_TYPE_WIN32_KMT_HANDLE_SUPPORTED;
const int CHPL_GPU_ATTRIBUTE__MAX_BLOCKS_PER_MULTIPROCESSOR = CU_DEVICE_ATTRIBUTE_MAX_BLOCKS_PER_MULTIPROCESSOR;
const int CHPL_GPU_ATTRIBUTE__GENERIC_COMPRESSION_SUPPORTED = CU_DEVICE_ATTRIBUTE_GENERIC_COMPRESSION_SUPPORTED;
const int CHPL_GPU_ATTRIBUTE__MAX_PERSISTING_L2_CACHE_SIZE = CU_DEVICE_ATTRIBUTE_MAX_PERSISTING_L2_CACHE_SIZE;
const int CHPL_GPU_ATTRIBUTE__MAX_ACCESS_POLICY_WINDOW_SIZE = CU_DEVICE_ATTRIBUTE_MAX_ACCESS_POLICY_WINDOW_SIZE;
const int CHPL_GPU_ATTRIBUTE__GPU_DIRECT_RDMA_WITH_CUDA_VMM_SUPPORTED = CU_DEVICE_ATTRIBUTE_GPU_DIRECT_RDMA_WITH_CUDA_VMM_SUPPORTED;
const int CHPL_GPU_ATTRIBUTE__RESERVED_SHARED_MEMORY_PER_BLOCK = CU_DEVICE_ATTRIBUTE_RESERVED_SHARED_MEMORY_PER_BLOCK;
const int CHPL_GPU_ATTRIBUTE__SPARSE_CUDA_ARRAY_SUPPORTED = CU_DEVICE_ATTRIBUTE_SPARSE_CUDA_ARRAY_SUPPORTED;
const int CHPL_GPU_ATTRIBUTE__READ_ONLY_HOST_REGISTER_SUPPORTED = CU_DEVICE_ATTRIBUTE_READ_ONLY_HOST_REGISTER_SUPPORTED;
const int CHPL_GPU_ATTRIBUTE__MEMORY_POOLS_SUPPORTED = CU_DEVICE_ATTRIBUTE_MEMORY_POOLS_SUPPORTED;
const int CHPL_GPU_ATTRIBUTE__GPU_DIRECT_RDMA_SUPPORTED = CU_DEVICE_ATTRIBUTE_GPU_DIRECT_RDMA_SUPPORTED;
const int CHPL_GPU_ATTRIBUTE__GPU_DIRECT_RDMA_FLUSH_WRITES_OPTIONS = CU_DEVICE_ATTRIBUTE_GPU_DIRECT_RDMA_FLUSH_WRITES_OPTIONS;
const int CHPL_GPU_ATTRIBUTE__GPU_DIRECT_RDMA_WRITES_ORDERING = CU_DEVICE_ATTRIBUTE_GPU_DIRECT_RDMA_WRITES_ORDERING;
const int CHPL_GPU_ATTRIBUTE__MEMPOOL_SUPPORTED_HANDLE_TYPES = CU_DEVICE_ATTRIBUTE_MEMPOOL_SUPPORTED_HANDLE_TYPES;
const int CHPL_GPU_ATTRIBUTE__DEFERRED_MAPPING_CUDA_ARRAY_SUPPORTED = CU_DEVICE_ATTRIBUTE_DEFERRED_MAPPING_CUDA_ARRAY_SUPPORTED;
*/

#endif // HAS_GPU_LOCALE
