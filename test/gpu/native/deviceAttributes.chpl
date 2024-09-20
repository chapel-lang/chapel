use GPU;

config const runBaseline = false;

extern {
  #if CHPL_GPU == amd
    #ifndef __HIP_PLATFORM_AMD__
    #define __HIP_PLATFORM_AMD__
    #endif
    #include <hip/hip_runtime.h>
    #include <hip/hip_runtime_api.h>
    #include <hip/hip_common.h>

    hipDevice_t device;

    static void reportAttribute(const char *name, hipDeviceAttribute_t attr) {
      int val;
      hipDeviceGetAttribute(&val, attr, device);
      printf("%s: %d\n", name, val);
    }

    static void runBaselineVersion(void) {
      hipInit(0);

      hipDeviceGet(&device, 0);

      int devCount;
      hipGetDeviceCount(&devCount);
      char name[0xFF];
      hipDeviceGetName(name, 0xFF, device);
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

  #endif
}

if runBaseline then runBaselineVersion();
else {
  on here.gpus[0] {
    writeln("name: ", deviceAttributes(here).name);
    writeln("maxThreadsPerBlock: ", deviceAttributes(here).maxThreadsPerBlock);
    writeln("maxBlockDimX: ", deviceAttributes(here).maxBlockDimX);
    writeln("maxBlockDimY: ", deviceAttributes(here).maxBlockDimY);
    writeln("maxBlockDimZ: ", deviceAttributes(here).maxBlockDimZ);
    writeln("MaxGridDimX: ", deviceAttributes(here).MaxGridDimX);
    writeln("maxGridDimY: ", deviceAttributes(here).maxGridDimY);
    writeln("maxGridDimZ: ", deviceAttributes(here).maxGridDimZ);
    writeln("maxSharedMemoryPerBlock: ", deviceAttributes(here).maxSharedMemoryPerBlock);
    writeln("totalConstantMemory: ", deviceAttributes(here).totalConstantMemory);
    writeln("warpSize: ", deviceAttributes(here).warpSize);
    writeln("maxPitch: ", deviceAttributes(here).maxPitch);
    writeln("maximumTexture1dWidth: ", deviceAttributes(here).maximumTexture1dWidth);
    writeln("maximumTexture2dWidth: ", deviceAttributes(here).maximumTexture2dWidth);
    writeln("maximumTexture2dHeight: ", deviceAttributes(here).maximumTexture2dHeight);
    writeln("maximumTexture3dWidth: ", deviceAttributes(here).maximumTexture3dWidth);
    writeln("maximumTexture3dHeight: ", deviceAttributes(here).maximumTexture3dHeight);
    writeln("maximumTexture3dDepth: ", deviceAttributes(here).maximumTexture3dDepth);
    writeln("maxRegistersPerBlock: ", deviceAttributes(here).maxRegistersPerBlock);
    writeln("clockRate: ", deviceAttributes(here).clockRate);
    writeln("textureAlignment: ", deviceAttributes(here).textureAlignment);
    writeln("texturePitch_alignment: ", deviceAttributes(here).texturePitch_alignment);
    writeln("multiprocessorCount: ", deviceAttributes(here).multiprocessorCount);
    writeln("kernelExecTimeout: ", deviceAttributes(here).kernelExecTimeout);
    writeln("integrated: ", deviceAttributes(here).integrated);
    writeln("canMapHostMemory: ", deviceAttributes(here).canMapHostMemory);
    writeln("computeMode: ", deviceAttributes(here).computeMode);
    writeln("concurrentKernels: ", deviceAttributes(here).concurrentKernels);
    writeln("eccEnabled: ", deviceAttributes(here).eccEnabled);
    writeln("pciBusId: ", deviceAttributes(here).pciBusId);
    writeln("pciDeviceId: ", deviceAttributes(here).pciDeviceId);
    writeln("memoryClockRate: ", deviceAttributes(here).memoryClockRate);
    writeln("globalMemoryBusWidth: ", deviceAttributes(here).globalMemoryBusWidth);
    writeln("l2CacheSize: ", deviceAttributes(here).l2CacheSize);
    writeln("maxThreadsPerMultiprocessor: ", deviceAttributes(here).maxThreadsPerMultiprocessor);
    writeln("computeCapabilityMajor: ", deviceAttributes(here).computeCapabilityMajor);
    writeln("computeCapabilityMinor: ", deviceAttributes(here).computeCapabilityMinor);
    writeln("maxSharedMemoryPerMultiprocessor: ", deviceAttributes(here).maxSharedMemoryPerMultiprocessor);
    writeln("managedMemory: ", deviceAttributes(here).managedMemory);
    writeln("multiGpuBoard: ", deviceAttributes(here).multiGpuBoard);
    writeln("pageableMemoryAccess: ", deviceAttributes(here).pageableMemoryAccess);
    writeln("concurrentManagedAccess: ", deviceAttributes(here).concurrentManagedAccess);
    writeln("pageableMemoryAccessUsesHostPageTables: ", deviceAttributes(here).pageableMemoryAccessUsesHostPageTables);
    writeln("directManagedMemAccessFromHost: ", deviceAttributes(here).directManagedMemAccessFromHost);
  }
}
