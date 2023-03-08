use GpuDiagnostics;

config const n = 100;

var x: int = 7;

proc foo(i) {
  return i - x;
}

startGpuDiagnostics();
on here.gpus[0] {
  var A: [0..#n] real;
  forall i in 0..#n {
    A[i] = foo(i);
  }
  writeln(A);
}
stopGpuDiagnostics();
writeln(getGpuDiagnostics());
