use GpuDiagnostics;

use GpuTestCommon;

config const n = 100;

proc foo(a, b, i) {
}


on here.gpus[0] {
  var A, B: [0..#n] real;
  startGpuDiagnostics();
  forall i in 0..#n {
    foo(A, B, i);
    //A[i] = B[i] + 1.0;
 }
  stopGpuDiagnostics();
}
verifyGpuDiags(umLaunch=1, aodLaunch=1);
