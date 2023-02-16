use GPUDiagnostics;

config const n = 10;
on here.gpus[0] {
  var A: [1..n] int;

  startGPUDiagnostics();
  A = 1;
  stopGPUDiagnostics();

  writeln(A);
}

assert(getGPUDiagnostics().kernel_launch == 1);
