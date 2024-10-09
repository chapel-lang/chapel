use GpuDiagnostics;

config const m = 1<<26,
             alpha = 3.0;

startVerboseGpu();
on here.gpus[0] {
  var A, B, C: [1..m] real;

  B = 1;
  C = 1;

  A = B + alpha*C;
}
stopVerboseGpu();
