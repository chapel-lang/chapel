use GPUDiagnostics;
config const n = 100;

proc foo(a, b, i) {
}


on here.getChild(1) {
  var A, B: [0..#n] real;
  startGPUDiagnostics();
  forall i in 0..#n {
    foo(A, B, i);
    //A[i] = B[i] + 1.0;
 }
  stopGPUDiagnostics();
}
writeln(getGPUDiagnostics());
