// Test for #20053

use GPUDiagnostics;

startGPUDiagnostics();

var Days : domain(int) = {0, 10, 20};
on here.gpus[0] {
  var A : [Days] real;

  // NOTE: Currently this does not generate a kernel launch
  foreach d in A.domain {
    A[d] = 10.0;
  }

  writeln(A);
}

stopGPUDiagnostics();
writeln("GPU diagnostics:");
writeln(getGPUDiagnostics());
