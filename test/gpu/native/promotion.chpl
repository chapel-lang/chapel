use GPUDiagnostics;

startGPUDiagnostics();
config const n = 10;
on here.gpus[0] {
  var A: [1..n] int;

  A = 1;

  writeln(A);
}
stopGPUDiagnostics();

assert(getGPUDiagnostics()[0].kernel_launch == 1);
