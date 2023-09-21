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

  forall i in 0..#n with (ref A) {
    A[i] = foo(i);
  }

  writeln(A);
}
stopGpuDiagnostics();
assertGpuDiags(kernel_launch_um=1, kernel_launch_aod=2);

var x = foo(13);
writeln(x);
