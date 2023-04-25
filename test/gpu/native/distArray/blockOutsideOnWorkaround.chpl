use BlockDist;
use GpuDiagnostics;

proc verifyLaunches() {
  use ChplConfig;
  const expected = if CHPL_GPU_MEM_STRATEGY == "unified_memory"
                      then here.maxTaskPar else here.maxTaskPar+1;
  const actual = getGpuDiagnostics()[0].kernel_launch;
  assert(actual == expected,
         "observed ", actual, " launches instead of ", expected);
}


config const n = here.maxTaskPar*2;

startGpuDiagnostics();

var space = {1..n};
var dom = space dmapped Block(space, targetLocales=[here.gpus[0],]);
var arr: [dom] int;

ref data = arr._value.myLocArr!.myElems;

forall i in dom do
  data[i] = 1;

stopGpuDiagnostics();

var sum = 0;
for a in arr do sum += a;
assert(sum == n);
verifyLaunches();
