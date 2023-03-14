use GpuDiagnostics;

proc verifyLaunches() {
  use ChplConfig;
  param expected = if CHPL_GPU_MEM_STRATEGY == "unified_memory" then 1 else 2;
  const actual = getGpuDiagnostics()[0].kernel_launch;
  assert(actual == expected,
         "observed ", actual, " launches instead of ", expected);
}

on here.gpus[0] {
  startGpuDiagnostics();
  var A : [0..0] (int,int);
  forall i in 0..0 {
    A[0] = createTuple();
  }
  stopGpuDiagnostics();
  verifyLaunches();
  writeln("A = ", A);

  proc createTuple() {
    return (123,456);
  }
}
