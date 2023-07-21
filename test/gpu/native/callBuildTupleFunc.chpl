use GpuDiagnostics;

use GpuTestCommon;

on here.gpus[0] {
  startGpuDiagnostics();
  var A : [0..0] (int,int);
  forall i in 0..0 {
    A[0] = createTuple();
  }
  stopGpuDiagnostics();
  verifyGpuDiags(umLaunch=1, aodLaunch=2);
  writeln("A = ", A);

  proc createTuple() {
    return (123,456);
  }
}
