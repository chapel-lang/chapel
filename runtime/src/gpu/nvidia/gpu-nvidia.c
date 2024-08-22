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

// #define CHPL_GPU_ENABLE_PROFILE // define this before including chpl-gpu.h

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
#include "chpl-env-gen.h"
#include "chpl-topo.h"

#include <cuda.h>
#include <cuda_runtime.h>
#include <assert.h>
#include <stdbool.h>


// this is compiler-generated
extern const char* chpl_gpuBinary;

static CUcontext *chpl_gpu_primary_ctx;
static CUdevice  *chpl_gpu_devices;

static int numAllDevices = -1;
static int numDevices = -1;
static int *deviceIDToIndex;

// array indexed by device ID (we load the same module once for each GPU).
static CUmodule *chpl_gpu_cuda_modules;

static int *deviceClockRates;

static bool chpl_gpu_has_context(void) {
  CUcontext cuda_context = NULL;

  CUresult ret = cuCtxGetCurrent(&cuda_context);

  if (ret == CUDA_ERROR_NOT_INITIALIZED || ret == CUDA_ERROR_DEINITIALIZED) {
    return false;
  }
  else {
    return cuda_context != NULL;
  }
}

static void switch_context(int dev_id) {
  CUcontext next_context = chpl_gpu_primary_ctx[dev_id];

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

static CUmodule get_module(void) {
  CUdevice device;
  CUmodule module;

  CUDA_CALL(cuCtxGetDevice(&device));
  assert((((int) device) >= 0) && (((int) device) < numAllDevices));
  int index = deviceIDToIndex[(int)device];
  assert((index >= 0) && (index < numDevices));
  module = chpl_gpu_cuda_modules[index];
  return module;
}

extern c_nodeid_t chpl_nodeID;

// we can put this logic in chpl-gpu.c. However, it needs to execute
// per-context/module. That's currently too low level for that layer.
static void chpl_gpu_impl_set_globals(c_sublocid_t dev_id, CUmodule module) {
  CUdeviceptr ptr;
  size_t glob_size;

  chpl_gpu_impl_load_global("chpl_nodeID", (void**)&ptr, &glob_size);

  assert(glob_size == sizeof(c_nodeid_t));
  chpl_gpu_impl_copy_host_to_device((void*)ptr, &chpl_nodeID, glob_size, NULL);
}

void chpl_gpu_impl_load_global(const char* global_name, void** ptr,
                               size_t* size) {
  CUmodule module = get_module();
  CUDA_CALL(cuModuleGetGlobal((CUdeviceptr*)ptr, size, module, global_name));
}

void* chpl_gpu_impl_load_function(const char* kernel_name) {
  CUfunction function;
  CUmodule module = get_module();
  CUDA_CALL(cuModuleGetFunction(&function, module, kernel_name));
  assert(function);

  return (void*)function;
}

void chpl_gpu_impl_use_device(c_sublocid_t dev_id) {
  switch_context(dev_id);
}

void chpl_gpu_impl_init(int* num_devices) {
  CUDA_CALL(cuInit(0));

  // Find all the GPUs (devices) on the machine then decide which we will
  // use. If there are co-locales then the GPUs are evenly divided among
  // them, otherwise we use them all.

  CUDA_CALL(cuDeviceGetCount(&numAllDevices));
  CUdevice *allDevices = chpl_malloc(sizeof(*allDevices) * numAllDevices);
  chpl_topo_pci_addr_t *allAddrs = chpl_malloc(sizeof(*allAddrs) * numAllDevices);

  // Find all the GPUs and get their PCI bus addresses.

  for (int i=0 ; i < numAllDevices; i++) {
    CUDA_CALL(cuDeviceGet(&allDevices[i], i));
    int domain, bus, device;
    CUDA_CALL(cuDeviceGetAttribute(&domain, CU_DEVICE_ATTRIBUTE_PCI_DOMAIN_ID,
                                   allDevices[i]));
    CUDA_CALL(cuDeviceGetAttribute(&bus, CU_DEVICE_ATTRIBUTE_PCI_BUS_ID,
                                   allDevices[i]));
    CUDA_CALL(cuDeviceGetAttribute(&device, CU_DEVICE_ATTRIBUTE_PCI_DEVICE_ID,
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

  // Allocate the GPU data structures. Note that the CUDA API, specifically
  // cuCtxGetDevice, returns the global device ID so we need deviceIDToIndex
  // to map from the global device ID to an array index.

  numDevices = numAddrs;
  chpl_gpu_primary_ctx = chpl_malloc(sizeof(CUcontext)*numDevices);
  chpl_gpu_devices = chpl_malloc(sizeof(CUdevice)*numDevices);
  chpl_gpu_cuda_modules = chpl_malloc(sizeof(CUmodule)*numDevices);
  deviceClockRates = chpl_malloc(sizeof(int)*numDevices);
  deviceIDToIndex = chpl_malloc(sizeof(int) * numAllDevices);

  // Go through the PCI bus addresses returned by chpl_topo_selectMyDevices
  // and find the corresponding GPUs. Initialize each GPU and its array
  // entries.

  int j = 0;
  for (int i = 0; i < numDevices; i++ ) {
    for (; j < numAllDevices; j++) {
      if (CHPL_TOPO_PCI_ADDR_EQUAL(&addrs[i], &allAddrs[j])) {
        CUdevice device = allDevices[j];

        CUcontext context;

        CUDA_CALL(cuDevicePrimaryCtxSetFlags(device,
                                             CU_CTX_SCHED_BLOCKING_SYNC));
        CUDA_CALL(cuDevicePrimaryCtxRetain(&context, device));

        CUDA_CALL(cuCtxSetCurrent(context));
        // load the module and setup globals within
        CUmodule module = chpl_gpu_load_module(chpl_gpuBinary);
        chpl_gpu_cuda_modules[i] = module;

        cuDeviceGetAttribute(&deviceClockRates[i],
                             CU_DEVICE_ATTRIBUTE_CLOCK_RATE, device);

        chpl_gpu_devices[i] = device;
        chpl_gpu_primary_ctx[i] = context;
        deviceIDToIndex[j] = i; // map device ID to array index

        // TODO can we refactor some of this to chpl-gpu to avoid duplication
        // between runtime layers?
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

bool chpl_gpu_impl_stream_supported(void) {
  return true;
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

void chpl_gpu_impl_launch_kernel(void* kernel,
                                 int grd_dim_x, int grd_dim_y, int grd_dim_z,
                                 int blk_dim_x, int blk_dim_y, int blk_dim_z,
                                 void* stream, void** kernel_params) {
  assert(kernel);

  CUDA_CALL(cuLaunchKernel((CUfunction)kernel,
                           grd_dim_x, grd_dim_y, grd_dim_z,
                           blk_dim_x, blk_dim_y, blk_dim_z,
                           0,       // shared memory in bytes
                           (CUstream)stream,  // stream ID
                           kernel_params,
                           NULL));  // extra options
}

void* chpl_gpu_impl_memset(void* addr, const uint8_t val, size_t n,
                           void* stream) {
  assert(chpl_gpu_is_device_ptr(addr));

  CUDA_CALL(cuMemsetD8Async((CUdeviceptr)addr, (unsigned int)val, n,
                            (CUstream)stream));

  return addr;
}

void chpl_gpu_impl_copy_device_to_host(void* dst, const void* src, size_t n,
                                       void* stream) {
  assert(chpl_gpu_is_device_ptr(src));

  CUDA_CALL(cuMemcpyDtoHAsync(dst, (CUdeviceptr)src, n, (CUstream)stream));
}

void chpl_gpu_impl_copy_host_to_device(void* dst, const void* src, size_t n,
                                       void* stream) {
  assert(chpl_gpu_is_device_ptr(dst));

  CUDA_CALL(cuMemcpyHtoDAsync((CUdeviceptr)dst, src, n, (CUstream)stream));
}

void chpl_gpu_impl_copy_device_to_device(void* dst, const void* src, size_t n,
                                         void* stream) {
  assert(chpl_gpu_is_device_ptr(dst) && chpl_gpu_is_device_ptr(src));

  CUDA_CALL(cuMemcpyDtoDAsync((CUdeviceptr)dst, (CUdeviceptr)src, n,
                              (CUstream)stream))
}


void* chpl_gpu_impl_comm_async(void *dst, void *src, size_t n) {
  CUstream stream;
  cuStreamCreate(&stream, CU_STREAM_NON_BLOCKING);
  cuMemcpyAsync((CUdeviceptr)dst, (CUdeviceptr)src, n, stream);
  return stream;
}

void chpl_gpu_impl_comm_wait(void *stream) {
  cuStreamSynchronize((CUstream)stream);
  cuStreamDestroy((CUstream)stream);
}

void* chpl_gpu_impl_mem_array_alloc(size_t size) {
  assert(size>0);

  CUdeviceptr ptr = 0;

#ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
  CUDA_CALL(cuMemAlloc(&ptr, size));
#else
  CUDA_CALL(cuMemAllocManaged(&ptr, size, CU_MEM_ATTACH_GLOBAL));
#endif

  return (void*)ptr;
}


void* chpl_gpu_impl_mem_alloc(size_t size) {
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
  if (memAlloc != NULL) {
    assert(chpl_gpu_is_device_ptr(memAlloc));
#ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
    if (chpl_gpu_impl_is_host_ptr(memAlloc)) {
      CUDA_CALL(cuMemFreeHost(memAlloc));
    }
    else {
      CUDA_CALL(cuMemFree((CUdeviceptr)memAlloc));
#else
    CUDA_CALL(cuMemFree((CUdeviceptr)memAlloc));
#endif

#ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
    }
#endif
  }
}

void chpl_gpu_impl_hostmem_register(void *memAlloc, size_t size) {
  // The CUDA driver uses DMA to transfer page-locked memory to the GPU; if
  // memory is not page-locked it must first be transferred into a page-locked
  // buffer, which degrades performance. So in the array_on_device mode we
  // choose to page-lock such memory even if it's on the host-side.
  #ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
  cudaHostRegister(memAlloc, size, cudaHostRegisterPortable);
  #endif
}

// This can be used for proper reallocation
size_t chpl_gpu_impl_get_alloc_size(void* ptr) {
  return chpl_gpu_common_get_alloc_size(ptr);
}

unsigned int chpl_gpu_device_clock_rate(int32_t devNum) {
  return (unsigned int)deviceClockRates[devNum];
}

bool chpl_gpu_impl_can_access_peer(int dev1, int dev2) {
  int p2p;
  CUDA_CALL(cuDeviceCanAccessPeer(&p2p, chpl_gpu_devices[dev1],
    chpl_gpu_devices[dev2]));
  return p2p != 0;
}

void chpl_gpu_impl_set_peer_access(int dev1, int dev2, bool enable) {
  switch_context(dev1);
  if(enable) {
    CUDA_CALL(cuCtxEnablePeerAccess(chpl_gpu_primary_ctx[dev2], 0));
  } else {
    CUDA_CALL(cuCtxDisablePeerAccess(chpl_gpu_primary_ctx[dev2]));
  }
}

void chpl_gpu_impl_synchronize(void) {
  CUDA_CALL(cuCtxSynchronize());
}

void* chpl_gpu_impl_stream_create(void) {
  CUstream stream;
  CUDA_CALL(cuStreamCreate(&stream, CU_STREAM_DEFAULT));
  return (void*) stream;
}

void chpl_gpu_impl_stream_destroy(void* stream) {
  if (stream) {
    CUDA_CALL(cuStreamDestroy((CUstream)stream));
  }
}

bool chpl_gpu_impl_stream_ready(void* stream) {
  if (stream) {
    CUresult res = cuStreamQuery(stream);
    if (res == CUDA_ERROR_NOT_READY) {
      return false;
    }
    CUDA_CALL(res);
  }
  return true;
}

void chpl_gpu_impl_stream_synchronize(void* stream) {
  if (stream) {
    CUDA_CALL(cuStreamSynchronize(stream));
  }
}

bool chpl_gpu_impl_can_reduce(void) {
  return true;
}

bool chpl_gpu_impl_can_sort(void){
  return true;
}

void* chpl_gpu_impl_host_register(void* var, size_t size) {
  CUDA_CALL(cuMemHostRegister(var, size, CU_MEMHOSTREGISTER_PORTABLE));
  return var;
}

void chpl_gpu_impl_host_unregister(void* var) {
  CUDA_CALL(cuMemHostUnregister(var));
}

#endif // HAS_GPU_LOCALE
