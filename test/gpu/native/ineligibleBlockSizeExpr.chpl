// This test demonstrates that the blockSize of a loop can contain GPU-ineligible
// code, since the blockSize is computed on the CPU before the kernel launch.

use GpuDiagnostics;

startVerboseGpu();

pragma "no gpu codegen"
proc computeBlockSize(input: int) {
  return input;
}

on here.gpus[0] {
  for size in [128, 256,512] {
    @gpu.blockSize(computeBlockSize(size))
    @assertOnGpu
    foreach 1..1024 {}
  }
}
