use GPU, GpuDiagnostics;

startVerboseGpu();

on here.gpus[0] {
  for j in 1..128 {
    foreach i in 0..0 {
      __primitive("gpu set blockSize", j);
    }
  }

  // Make sure the user-facing functions for setting block size invoke the prim.
  for j in 1..128 {
    foreach i in 0..0 {
      setBlockSize(j);
    }
  }

  for j in 1..128 {
    @gpu.blockSize(j)
    foreach i in 0..0 {}
  }
}

stopVerboseGpu();
