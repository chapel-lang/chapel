use BlockDist;
use GPUDiagnostics;

config const n = 10;

startGPUDiagnostics();

var space = {1..n};
var dom = space dmapped Block(space, targetLocales=[here.gpus[0],]);
var arr: [dom] int;

ref data = arr._value.myLocArr!.myElems;

forall i in dom do
  data[i] = 1;

stopGPUDiagnostics();

writeln(arr);
writeln(getGPUDiagnostics());
