use BlockDist;
use GpuDiagnostics;
use CTypes;

config const printArr = false;
config const n = here.maxTaskPar*2;

startGpuDiagnostics();

var space = {1..n};
var dom = space dmapped new blockDist(space, targetLocales=[here.gpus[0],]);
var arr: [dom] int;

proc setOne(ref A, i) {
  A[i] = 1;
}

@assertOnGpu
forall i in dom do
  setOne(arr, i);

stopGpuDiagnostics();

var hostArr: [space] int;
hostArr = arr;

if printArr then writeln(hostArr);

assert((+ reduce hostArr) == n);
assertGpuDiags(kernel_launch_um=here.maxTaskPar,
               kernel_launch_aod=here.maxTaskPar+1);
