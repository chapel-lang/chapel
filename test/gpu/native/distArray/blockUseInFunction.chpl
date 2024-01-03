use BlockDist;
use GpuDiagnostics;
use CTypes;

/*pragma "codegen for GPU"*/
/*extern const chpl_privateObjects: c_ptr(void);*/

config const n = here.maxTaskPar*2;

startGpuDiagnostics();

var space = {1..n};
var dom = space dmapped blockDist(space, targetLocales=[here.gpus[0],]);
var arr: [dom] int;

proc setOne(A, i) {
  local {
    A[i] = 1;
  }
}

@assertOnGpu
forall i in dom do
  setOne(arr, i);

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
