use GpuDiagnostics;
use MemDiagnostics;


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

assertGpuDiags(kernel_launch=1, host_to_device=1,device_to_host=1,
               device_to_device=0);
resetGpuDiagnostics();
assertGpuDiags(kernel_launch=0, host_to_device=0, device_to_host=0,
               device_to_device=0);
