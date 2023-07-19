use GpuDiagnostics;

use GpuTestCommon;

on here.gpus[0] {
  startGpuDiagnostics();
  var A : [0..0] (int,int);
  forall i in 0..0 {
    A[0] = createTuple();
  }
  stopGpuDiagnostics();
  verifyDiags(um=1, aod=2);
  writeln("A = ", A);

  proc createTuple() {
    return (123,456);
  }
}
