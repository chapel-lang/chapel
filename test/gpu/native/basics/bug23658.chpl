// this code is from https://github.com/chapel-lang/chapel/issues/23658 verbatim
// the bug reported will be resolved via
// TBD
use GpuDiagnostics;

config const gpuDiags = true;
const sz = 10;

if gpuDiags then startGpuDiagnostics();

on here.gpus[0] {
  var Arr: [0..<sz] int;
  foreach i in 1..0 do {
    Arr[i] = i;
  }
  writeln(Arr);
}

if gpuDiags {
  stopGpuDiagnostics();
  assertGpuDiags(kernel_launch=2, host_to_device=0, device_to_host=sz,
                 device_to_device=0);
}
