use GpuDiagnostics;

config const n = 16;

const numGpus = here.gpus.size;
assert(n%numGpus == 0);

const perGpuSize = n/numGpus;

// Workaround for now. See https://github.com/chapel-lang/chapel/issues/23650
inline proc sublocId {
  return ((here._instance):GPULocale).sid;
}

startGpuDiagnostics();

{
  var CpuVec: [1..n] int;

  coforall gpu in here.gpus do on gpu {
    const myChunk = 1+sublocId*perGpuSize..#perGpuSize;

    var GpuVec = CpuVec[myChunk];
    GpuVec += 1;
    CpuVec[myChunk] = GpuVec;
  }

  writeln(CpuVec);
}

stopGpuDiagnostics();
assertGpuDiags(kernel_launch=numGpus, host_to_device=numGpus,
               device_to_host=numGpus);
