use GpuDiagnostics;
use MemDiagnostics;

writeln("Start");

startVerboseGpu();
startGpuDiagnostics();
startVerboseMem();

var A: [1..10] int = 1;

on here.gpus[0] {
  var AonGPU = A;
  foreach a in AonGPU {
    a += 1;
  }

  A = AonGPU;
}

stopVerboseMem();
stopGpuDiagnostics();
stopVerboseGpu();

writeln(A);
writeln("End");

writeln("GPU diagnostics:");
writeln(getGpuDiagnostics());

resetGpuDiagnostics();
writeln("GPU diagnostics after reset:");
writeln(getGpuDiagnostics());
