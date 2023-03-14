use GpuDiagnostics;

proc verifyLaunches() {
  use ChplConfig;
  param expected = if CHPL_GPU_MEM_STRATEGY == "unified_memory" then 1 else 2;
  const actual = getGpuDiagnostics()[0].kernel_launch;
  assert(actual == expected,
         "observed ", actual, " launches instead of ", expected);
}

config const n = 10;

proc foo(i) {
  return 1 + bar(i);
}

proc bar(i) {
  return i / 2;
}

startGpuDiagnostics();
on here.gpus[0] {
  var A: [0..#n] int;

  forall i in 0..#n {
    A[i] = foo(i);
  }

  writeln(A);
}
stopGpuDiagnostics();
verifyLaunches();

var x = foo(13);
writeln(x);
