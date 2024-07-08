use GpuDiagnostics;

config const n = 16;
assert(n%here.gpus.size == 0);
const numGpusPerLoc = here.gpus.size;
const perGpuSize = n/numGpusPerLoc/numLocales;

// Workaround for now. See https://github.com/chapel-lang/chapel/issues/23650
inline proc sublocId {
  return ((here._instance):GPULocale).sid;
}

startGpuDiagnostics();

{
  var CpuVec: [1..n] int;
  coforall loc in Locales do on loc {
    coforall gpu in here.gpus do on gpu {
      const myChunk = 1+(numGpusPerLoc*here.id+sublocId)*perGpuSize..#perGpuSize;

      var GpuVec = CpuVec[myChunk];
      GpuVec += 1;
      CpuVec[myChunk] = GpuVec;
    }
  }
  writeln(CpuVec);
}

stopGpuDiagnostics();

const totalNumGpus = numLocales*numGpusPerLoc;
assertGpuDiags(kernel_launch=totalNumGpus,
               host_to_device=totalNumGpus,
               device_to_host=totalNumGpus);
