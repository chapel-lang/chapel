use GpuDiagnostics;
config const n = 10;

proc foo(i) {
  return 1 + bar(i);
}

proc bar(i) {
  return i / 2;
}

startGpuDiagnostics();
on here.gpus[0] {
  var A: [0..#n] int;

  forall i in 0..#n {
    A[i] = foo(i);
  }

  writeln(A);
}
stopGpuDiagnostics();
writeln(getGpuDiagnostics());

var x = foo(13);
writeln(x);
