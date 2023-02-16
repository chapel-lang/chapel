use GPUDiagnostics;

on here.gpus[0] {
  var A : [0..10] real(32);
  startGPUDiagnostics();
  foreach i in 0..10 {
    A[i] = i;
    if i == 3 { break;}
  }
  stopGPUDiagnostics();
  writeln(A);
}

writeln(getGPUDiagnostics());
