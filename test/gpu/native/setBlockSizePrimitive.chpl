use GPU, GpuDiagnostics;

startVerboseGpu();

on here.gpus[0] {
  for j in 1..128 {
    @gpu.blockSize(j)
    foreach i in 0..0 {}
  }
}

stopVerboseGpu();
