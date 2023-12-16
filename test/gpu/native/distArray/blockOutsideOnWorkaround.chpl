use BlockDist;
use GpuDiagnostics;


config const n = here.maxTaskPar*2;

startGpuDiagnostics();
startVerboseGpu();

var space = {1..n};
var dom = space dmapped blockDist(space, targetLocales=[here.gpus[0],]);
var arr: [dom] int;

/*writeln(__primitive("size of", arr._instance));*/
/*writeln(numBytes(arr._instance.type));*/

writeln("100");
ref data = arr._value.myLocArr!.myElems;
writeln("200");

@assertOnGpu
forall i in dom do
  arr[i] = 1;
writeln("300");

stopVerboseGpu();
stopGpuDiagnostics();

var hostArr: [space] int;
hostArr = arr;
writeln(hostArr);

// TODO this following part is where we have the problem with AOD
/*var sum = 0;*/
/*for a in arr do sum += a;*/
/*assert(sum == n);*/
assertGpuDiags(kernel_launch_um=here.maxTaskPar,
               kernel_launch_aod=here.maxTaskPar+1);
