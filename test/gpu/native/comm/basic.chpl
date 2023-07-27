use GpuDiagnostics;


config const printArrs = false;
config const verboseGpu = false;
config const gpuDiags = true;

if gpuDiags then startGpuDiagnostics();
if verboseGpu then startVerboseGpu();
config const n = 10;

var parent = here;
on here.gpus[0] {
  var A: [1..n] int = 1;  // kernel launch
  if printArrs then writeln(A);

  on parent {
    var B = A;  // device to host copy
    if printArrs then writeln(B);

    B += 1;  // not a kernel launch

    on here.gpus[0] {
      var C = B;  // host to device copy;
      if printArrs then writeln(C);

      C += 1;  // kernel launch

      A = C;  // device to device copy
    }
  }
}
if verboseGpu then stopVerboseGpu();
if gpuDiags {
  stopGpuDiagnostics();
  assertGpuDiags(kernel_launch=2, host_to_device=1, device_to_host=1,
                 device_to_device=1);
}
