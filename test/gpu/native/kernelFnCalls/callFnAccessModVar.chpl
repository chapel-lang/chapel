use GpuDiagnostics;


config const n = 100;

var x: int = 7;

proc foo(i) {
  return i - x;
}

on here.gpus[0] {
  var A: [0..#n] real;
  startGpuDiagnostics();
  forall i in 0..#n with (ref A) {
    A[i] = foo(i);
  }
  stopGpuDiagnostics();
  writeln(A);
}
assertGpuDiags(kernel_launch=1);
