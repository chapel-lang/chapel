use GpuDiagnostics;


on here.gpus[0] {
  startGpuDiagnostics();
  var A : [0..0] (int,int);
  forall i in 0..0 with (ref A) {
    A[0] = createTuple();
  }
  stopGpuDiagnostics();
  assertGpuDiags(kernel_launch_um=1, kernel_launch_aod=2);
  writeln("A = ", A);

  proc createTuple() {
    return (123,456);
  }
}
