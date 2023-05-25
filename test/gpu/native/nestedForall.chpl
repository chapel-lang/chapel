use GpuDiagnostics;
use GPU;

proc verifyLaunches() {
  use ChplConfig;
  param expected = if CHPL_GPU_MEM_STRATEGY == "unified_memory" then 2 else 4;
  const actual = getGpuDiagnostics()[0].kernel_launch;
  assert(actual == expected,
         "observed ", actual, " launches instead of ", expected);
}

config const low = 0, high = -1;

startGpuDiagnostics();
on here.gpus[0] {
  var A: [1..10, 1..10] int;
  forall a in A {
    assertOnGpu();
    a += 1;
  }
  writeln(A);
  var B: [1..10, 1..10] real;
  forall (i,j) in {1..10, 1..10} {
    assertOnGpu();
    B(i,j) = i + j;
  }
  writeln(B);
  forall (i,j) in {1..-1, 1..-1} {
    assertOnGpu();
  }
  forall (i,j) in {low..high, low..high} {
    assertOnGpu();
  }
}
stopGpuDiagnostics();
verifyLaunches();
