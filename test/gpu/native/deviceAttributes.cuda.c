#include "deviceAttributes.h"
#include <cuda.h>
#include <stdio.h>

#define CUDA_CALL(call) do {\
  if(call) { \
    printf("!!! error encountered on ROCM call\n"); \
  } \
} while(0);

CUdevice device;

static void reportAttribute(const char *name, CUdevice_attribute attr) {
  int val;
  CUDA_CALL(cuDeviceGetAttribute(&val, attr, device));
  printf("%s: %d\n", name, val);
}

void runBaselineVersion(void) {
  CUDA_CALL(cuInit(0));
  CUDA_CALL(cuDeviceGet(&device, 0));

  char name[0xFF];
  CUDA_CALL(cuDeviceGetName(name, 0xFF, device));
  printf("name: %s\n", name);

  reportAttribute("maxThreadsPerBlock", CU_DEVICE_ATTRIBUTE_MAX_THREADS_PER_BLOCK);
  reportAttribute("maxBlockDimX", CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_X);
  reportAttribute("maxBlockDimY", CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_Y);
  reportAttribute("maxBlockDimZ", CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_Z);
  reportAttribute("MaxGridDimX", CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_X);
  reportAttribute("maxGridDimY", CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_Y);
  reportAttribute("maxGridDimZ", CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_Z);
  reportAttribute("maxSharedMemoryPerBlock", CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_BLOCK);
  reportAttribute("totalConstantMemory", CU_DEVICE_ATTRIBUTE_TOTAL_CONSTANT_MEMORY);
  reportAttribute("warpSize", CU_DEVICE_ATTRIBUTE_WARP_SIZE);
  reportAttribute("maxPitch", CU_DEVICE_ATTRIBUTE_MAX_PITCH);
  reportAttribute("maximumTexture1dWidth", CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE1D_WIDTH);
  reportAttribute("maximumTexture2dWidth", CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_WIDTH);
  reportAttribute("maximumTexture2dHeight", CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_HEIGHT);
  reportAttribute("maximumTexture3dWidth", CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE3D_WIDTH);
  reportAttribute("maximumTexture3dHeight", CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE3D_HEIGHT);
  reportAttribute("maximumTexture3dDepth", CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE3D_DEPTH);
  reportAttribute("maxRegistersPerBlock", CU_DEVICE_ATTRIBUTE_MAX_REGISTERS_PER_BLOCK);
  reportAttribute("clockRate", CU_DEVICE_ATTRIBUTE_CLOCK_RATE);
  reportAttribute("textureAlignment", CU_DEVICE_ATTRIBUTE_TEXTURE_ALIGNMENT);
  reportAttribute("texturePitch_alignment", CU_DEVICE_ATTRIBUTE_TEXTURE_PITCH_ALIGNMENT);
  reportAttribute("multiprocessorCount", CU_DEVICE_ATTRIBUTE_MULTIPROCESSOR_COUNT);
  reportAttribute("kernelExecTimeout", CU_DEVICE_ATTRIBUTE_KERNEL_EXEC_TIMEOUT);
  reportAttribute("integrated", CU_DEVICE_ATTRIBUTE_INTEGRATED);
  reportAttribute("canMapHostMemory", CU_DEVICE_ATTRIBUTE_CAN_MAP_HOST_MEMORY);
  reportAttribute("computeMode", CU_DEVICE_ATTRIBUTE_COMPUTE_MODE);
  reportAttribute("concurrentKernels", CU_DEVICE_ATTRIBUTE_CONCURRENT_KERNELS);
  reportAttribute("eccEnabled", CU_DEVICE_ATTRIBUTE_ECC_ENABLED);
  reportAttribute("pciBusId", CU_DEVICE_ATTRIBUTE_PCI_BUS_ID);
  reportAttribute("pciDeviceId", CU_DEVICE_ATTRIBUTE_PCI_DEVICE_ID);
  reportAttribute("memoryClockRate", CU_DEVICE_ATTRIBUTE_MEMORY_CLOCK_RATE);
  reportAttribute("globalMemoryBusWidth", CU_DEVICE_ATTRIBUTE_GLOBAL_MEMORY_BUS_WIDTH);
  reportAttribute("l2CacheSize", CU_DEVICE_ATTRIBUTE_L2_CACHE_SIZE);
  reportAttribute("maxThreadsPerMultiprocessor", CU_DEVICE_ATTRIBUTE_MAX_THREADS_PER_MULTIPROCESSOR);
  reportAttribute("computeCapabilityMajor", CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR);
  reportAttribute("computeCapabilityMinor", CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR);
  reportAttribute("maxSharedMemoryPerMultiprocessor", CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_MULTIPROCESSOR);
  reportAttribute("managedMemory", CU_DEVICE_ATTRIBUTE_MANAGED_MEMORY);
  reportAttribute("multiGpuBoard", CU_DEVICE_ATTRIBUTE_MULTI_GPU_BOARD);
  reportAttribute("pageableMemoryAccess", CU_DEVICE_ATTRIBUTE_PAGEABLE_MEMORY_ACCESS);
  reportAttribute("concurrentManagedAccess", CU_DEVICE_ATTRIBUTE_CONCURRENT_MANAGED_ACCESS);
  reportAttribute("pageableMemoryAccessUsesHostPageTables", CU_DEVICE_ATTRIBUTE_PAGEABLE_MEMORY_ACCESS_USES_HOST_PAGE_TABLES);
  reportAttribute("directManagedMemAccessFromHost", CU_DEVICE_ATTRIBUTE_DIRECT_MANAGED_MEM_ACCESS_FROM_HOST);
}
