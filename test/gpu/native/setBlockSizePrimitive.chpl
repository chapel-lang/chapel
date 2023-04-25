use GpuDiagnostics;

startVerboseGpu();

on here.gpus[0] {
  for j in 1..128 {
    foreach i in 0..0 {
      __primitive("gpu set blockSize", j);
    }
  }
}

stopVerboseGpu();
