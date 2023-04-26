use GpuDiagnostics;

config const n = 10;
on here.gpus[0] {
  var A: [1..n] int;

  startGpuDiagnostics();
  A = 1;
  stopGpuDiagnostics();

  writeln(A);
}

assert(getGpuDiagnostics().kernel_launch == 1);
