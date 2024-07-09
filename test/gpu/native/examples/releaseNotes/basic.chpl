use GpuDiagnostics;

config const n = 10;

startGpuDiagnostics();

{
  on here.gpus[0] {
    var GpuVec: [1..n] int;
    GpuVec += 1;
    writeln(GpuVec);
  }
}

stopGpuDiagnostics();
assertGpuDiags(kernel_launch_aod=2, kernel_launch_um=1);
