// This test demonstrates that the blockSize of a loop can contain GPU-ineligible
// code, since the blockSize is computed on the CPU before the kernel launch.

use GpuDiagnostics;

startVerboseGpu();

pragma "no gpu codegen"
proc computeBlockSize(input: int) {
  return input;
}

on here.gpus[0] {
    @gpu.blockSize(computeBlockSize(128))
    @assertOnGpu
    foreach 1..1024 {}

    @gpu.blockSize(computeBlockSize(256))
    @assertOnGpu
    foreach 1..1024 {}

    @gpu.blockSize(computeBlockSize(512))
    @assertOnGpu
    foreach 1..1024 {}
}
