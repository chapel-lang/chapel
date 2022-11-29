module ChapelGpuSupport {
  use ChapelBase;

  extern var chpl_gpu_debug: bool;

  config const debugGpu = false;

  // by virtue of module initialization:
  chpl_gpu_debug = debugGpu;
}
