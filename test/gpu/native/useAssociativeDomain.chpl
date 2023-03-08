// Test for #20053

use GpuDiagnostics;

startGpuDiagnostics();

var Days : domain(int) = {0, 10, 20};
on here.gpus[0] {
  var A : [Days] real;

  // NOTE: Currently this does not generate a kernel launch
  foreach d in A.domain {
    A[d] = 10.0;
  }

  writeln(A);
}

stopGpuDiagnostics();
writeln("GPU diagnostics:");
writeln(getGpuDiagnostics());
