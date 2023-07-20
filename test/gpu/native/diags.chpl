use GpuDiagnostics;
use MemDiagnostics;

use GpuTestCommon;

// verbose gpu diagnostics have different behavior based on mem strategy
config const verboseDiags = false;

writeln("Start");

if verboseDiags then startVerboseGpu();
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
if verboseDiags then stopVerboseGpu();

writeln(A);
writeln("End");

verifyDiags(um=1, aod=1, h2d=1, d2h=1, d2d=1);
resetGpuDiagnostics();
verifyDiags(um=0, aod=0, h2d=0, d2h=0, d2d=0);
