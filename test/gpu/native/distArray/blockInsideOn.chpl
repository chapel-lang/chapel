use BlockDist;
use GpuDiagnostics;


config const n = 10;

startGpuDiagnostics();
on here.gpus[0] {
  var space = {1..n};
  var dom = space dmapped new blockDist(space, targetLocales=[here,]);
  var arr: [dom] int;

  forall i in dom with (ref arr) do
    arr[i] = 1;

  /* The following does not work yet:

    forall a in arr do
      a = 1;

  */

  writeln(arr[1]);

  /* The following does not work yet:

     writeln(arr);

  */
}

stopGpuDiagnostics();

assertGpuDiags(kernel_launch_um=4, kernel_launch_aod=6);
