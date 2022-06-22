use GPUDiagnostics;

startGPUDiagnostics();
on here.gpus[0] {
  var A: [1..10] real;
  foreach i in A.domain {
    A[i] += sin(i);
  }

  writeln(A);
}
stopGPUDiagnostics();
assert(getGPUDiagnostics().kernel_launch == 1);
