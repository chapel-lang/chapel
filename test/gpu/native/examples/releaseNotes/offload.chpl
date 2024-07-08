use GpuDiagnostics;

config const n = 16;
assert(n%here.gpus.size == 0);
const perGpuSize = n/here.gpus.size;

// Workaround for now. See https://github.com/chapel-lang/chapel/issues/23650
inline proc sublocId {
  return ((here._instance):GPULocale).sid;
}

startGpuDiagnostics();

{
  var CpuVec: [1..n] int;

  on here.gpus[0] {
    var GpuVec = CpuVec;
    GpuVec += 1;
    CpuVec = GpuVec;
  }

  writeln(CpuVec);
}

stopGpuDiagnostics();
assertGpuDiags(kernel_launch=1, host_to_device=1, device_to_host=1);
