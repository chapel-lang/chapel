use BlockDist;
use GpuDiagnostics;

proc verifyLaunches() {
  use ChplConfig;
  param expected = if CHPL_GPU_MEM_STRATEGY == "unified_memory" then 4 else 6;
  const actual = getGpuDiagnostics()[0].kernel_launch;
  assert(actual == expected,
         "observed ", actual, " launches instead of ", expected);
}

config const n = 10;

startGpuDiagnostics();
on here.gpus[0] {
  var space = {1..n};
  var dom = space dmapped Block(space, targetLocales=[here,]);
  var arr: [dom] int;

  forall i in dom do
    arr[i] = 1;

  /* The following does not work yet:

    forall a in arr do
      a = 1;

  */

  writeln(arr);
}

stopGpuDiagnostics();

verifyLaunches();
