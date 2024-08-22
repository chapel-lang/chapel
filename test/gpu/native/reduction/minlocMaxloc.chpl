use GpuDiagnostics;

on here.gpus[0] {
  var A:[1..10] int;

  startGpuDiagnostics();
  writeln(minloc reduce zip(A, A.domain));
  writeln(maxloc reduce zip(A, A.domain));
  stopGpuDiagnostics();
}

assertGpuDiags(kernel_launch=0);
