use BlockDist;
use GpuDiagnostics;


config const n = 20;

startGpuDiagnostics();

var space = {1..n};
var dom = space dmapped blockDist(space, targetLocales=[here.gpus[0],]);
var arr: [dom] int;

@assertOnGpu
forall i in dom do
  arr[i] = 1;

stopGpuDiagnostics();

var hostArr: [space] int;
hostArr = arr;
writeln(hostArr);

assertGpuDiags(kernel_launch_um=here.maxTaskPar,
               kernel_launch_aod=here.maxTaskPar+1);
