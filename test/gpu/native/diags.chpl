use GPUDiagnostics;
use Memory.Diagnostics;

writeln("Start");

startVerboseGPU();
startGPUDiagnostics();
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
stopGPUDiagnostics();
stopVerboseGPU();

writeln(A);
writeln("End");

writeln("GPU diagnostics:");
writeln(getGPUDiagnostics());

resetGPUDiagnostics();
writeln("GPU diagnostics after reset:");
writeln(getGPUDiagnostics());
