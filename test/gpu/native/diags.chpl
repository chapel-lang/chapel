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

verifyGpuDiags(umLaunch=1, aodLaunch=1, h2dComm=1, d2hComm=1, d2dComm=0);
resetGpuDiagnostics();
verifyGpuDiags(umLaunch=0, aodLaunch=0, h2dComm=0, d2hComm=0, d2dComm=0);
