use GPU;
use GPUDiagnostics;
config const N = 100;

on here.gpus[0] {
  var A : [0..<N] int;
  startGPUDiagnostics();
  foreach i in 0..<N {
    assertOnGpu();
    A[i] = i * 2;
  }
  stopGPUDiagnostics();
  writeln("GPU Diags: ", getGPUDiagnostics());
  writeln(A);
}
