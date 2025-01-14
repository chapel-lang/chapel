use GpuDiagnostics;

on here.gpus[0] {
  startVerboseGpu();
  for size in 128..1024 by 128 {
    @gpu.blockSize(size)
    foreach i in 1..1500 { }
  }
  stopVerboseGpu();
}
