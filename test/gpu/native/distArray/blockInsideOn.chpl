use BlockDist;
use GpuDiagnostics;


config const n = 10;

startGpuDiagnostics();
on here.gpus[0] {
  var Space = {1..n};
  var Dom = Space dmapped blockDist(Space, targetLocales=[here,]);
  var Arr: [Dom] int;

  @assertOnGpu
  foreach i in Dom do
    Arr[i] = 1;

  /* The following does not work yet:

    forall a in arr do
      a = 1;

  */

  writeln(Arr);
}

stopGpuDiagnostics();

assertGpuDiags(kernel_launch_um=4, kernel_launch_aod=6);
