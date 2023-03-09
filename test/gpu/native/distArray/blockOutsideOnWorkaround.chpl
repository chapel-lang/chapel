use BlockDist;
use GpuDiagnostics;

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
assert(here.maxTaskPar == getGpuDiagnostics().kernel_launch);
