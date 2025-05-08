use GPU;

config const runBaseline = false;

extern proc runBaselineVersion();

if runBaseline then runBaselineVersion();
else {
  on here.gpus[0] {
    writeln("name: ", deviceAttributes(here).name);
    writeln("maxThreadsPerBlock: ", deviceAttributes(here).maxThreadsPerBlock);
    writeln("maxBlockDimX: ", deviceAttributes(here).maxBlockDimX);
    writeln("maxBlockDimY: ", deviceAttributes(here).maxBlockDimY);
    writeln("maxBlockDimZ: ", deviceAttributes(here).maxBlockDimZ);
    writeln("maxGridDimX: ", deviceAttributes(here).maxGridDimX);
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
