#include "deviceAttributes.h"

#ifndef __HIP_PLATFORM_AMD__
#define __HIP_PLATFORM_AMD__
#endif
#include <hip/hip_runtime.h>
#include <hip/hip_runtime_api.h>
#include <hip/hip_common.h>

#define ROCM_CALL(call) do {\
  if(call) { \
    printf("!!! error encountered on ROCM call\n"); \
  } \
} while(0);

hipDevice_t device;

static void reportAttribute(const char *name, hipDeviceAttribute_t attr) {
  int val;
  hipError_t err;
  ROCM_CALL(hipDeviceGetAttribute(&val, attr, device));
  printf("%s: %d\n", name, val);
}

void runBaselineVersion(void) {
  ROCM_CALL(hipInit(0));
  ROCM_CALL(hipDeviceGet(&device, 0));

  char name[0xFF];
  ROCM_CALL(hipDeviceGetName(name, 0xFF, device));
  printf("name: %s\n", name);

  reportAttribute("maxThreadsPerBlock", hipDeviceAttributeMaxThreadsPerBlock);
  reportAttribute("maxBlockDimX", hipDeviceAttributeMaxBlockDimX);
  reportAttribute("maxBlockDimY", hipDeviceAttributeMaxBlockDimY);
  reportAttribute("maxBlockDimZ", hipDeviceAttributeMaxBlockDimZ);
  reportAttribute("MaxGridDimX", hipDeviceAttributeMaxGridDimX);
  reportAttribute("maxGridDimY", hipDeviceAttributeMaxGridDimY);
  reportAttribute("maxGridDimZ", hipDeviceAttributeMaxGridDimZ);
  reportAttribute("maxSharedMemoryPerBlock", hipDeviceAttributeMaxSharedMemoryPerBlock);
  reportAttribute("totalConstantMemory", hipDeviceAttributeTotalConstantMemory);
  reportAttribute("warpSize", hipDeviceAttributeWarpSize);
  reportAttribute("maxPitch", hipDeviceAttributeMaxPitch);
  reportAttribute("maximumTexture1dWidth", hipDeviceAttributeMaxTexture1DWidth);
  reportAttribute("maximumTexture2dWidth", hipDeviceAttributeMaxTexture2DWidth);
  reportAttribute("maximumTexture2dHeight", hipDeviceAttributeMaxTexture2DHeight);
  reportAttribute("maximumTexture3dWidth", hipDeviceAttributeMaxTexture3DWidth);
  reportAttribute("maximumTexture3dHeight", hipDeviceAttributeMaxTexture3DHeight);
  reportAttribute("maximumTexture3dDepth", hipDeviceAttributeMaxTexture3DDepth);
  reportAttribute("maxRegistersPerBlock", hipDeviceAttributeMaxRegistersPerBlock);
  reportAttribute("clockRate", hipDeviceAttributeClockRate);
  reportAttribute("textureAlignment", hipDeviceAttributeTextureAlignment);
  reportAttribute("texturePitch_alignment", hipDeviceAttributeTexturePitchAlignment);
  reportAttribute("multiprocessorCount", hipDeviceAttributeMultiprocessorCount);
  reportAttribute("kernelExecTimeout", hipDeviceAttributeKernelExecTimeout);
  reportAttribute("integrated", hipDeviceAttributeIntegrated);
  reportAttribute("canMapHostMemory", hipDeviceAttributeCanMapHostMemory);
  reportAttribute("computeMode", hipDeviceAttributeComputeMode);
  reportAttribute("concurrentKernels", hipDeviceAttributeConcurrentKernels);
  reportAttribute("eccEnabled", hipDeviceAttributeEccEnabled);
  reportAttribute("pciBusId", hipDeviceAttributePciBusId);
  reportAttribute("pciDeviceId", hipDeviceAttributePciDeviceId);
  reportAttribute("memoryClockRate", hipDeviceAttributeMemoryClockRate);
  reportAttribute("globalMemoryBusWidth", hipDeviceAttributeMemoryBusWidth);
  reportAttribute("l2CacheSize", hipDeviceAttributeL2CacheSize);
  reportAttribute("maxThreadsPerMultiprocessor", hipDeviceAttributeMaxThreadsPerMultiProcessor);
  reportAttribute("computeCapabilityMajor", hipDeviceAttributeComputeCapabilityMajor);
  reportAttribute("computeCapabilityMinor", hipDeviceAttributeComputeCapabilityMinor);
  reportAttribute("maxSharedMemoryPerMultiprocessor", hipDeviceAttributeMaxSharedMemoryPerMultiprocessor);
  reportAttribute("managedMemory", hipDeviceAttributeManagedMemory);
  reportAttribute("multiGpuBoard", hipDeviceAttributeIsMultiGpuBoard);
  reportAttribute("pageableMemoryAccess", hipDeviceAttributePageableMemoryAccess);
  reportAttribute("concurrentManagedAccess", hipDeviceAttributeConcurrentManagedAccess);
  reportAttribute("pageableMemoryAccessUsesHostPageTables", hipDeviceAttributePageableMemoryAccessUsesHostPageTables);
  reportAttribute("directManagedMemAccessFromHost", hipDeviceAttributeDirectManagedMemAccessFromHost);
}
