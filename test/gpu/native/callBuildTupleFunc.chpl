use GPUDiagnostics;

on here.gpus[0] {
  startGPUDiagnostics();
  var A : [0..0] (int,int);
  forall i in 0..0 {
    A[0] = createTuple();
  }
  stopGPUDiagnostics();
  writeln(getGPUDiagnostics());
  writeln("A = ", A);

  proc createTuple() {
    return (123,456);
  }
}
