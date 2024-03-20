use GpuDiagnostics;

config const n = 64;
assert(n%here.gpus.size == 0);
config const numTasksPerGpu = 2;
const numGpusPerLoc = here.gpus.size;
const perTaskSize = n/numGpusPerLoc/numLocales/numTasksPerGpu;

// Workaround for now. See https://github.com/chapel-lang/chapel/issues/23650
inline proc sublocId {
  return ((here._instance):GPULocale).sid;
}

startGpuDiagnostics();

{
  var CpuVec: [1..n] int;
  coforall loc in Locales do on loc {
    coforall gpu in here.gpus do on gpu {
      // just to avoid messy math later on
      const gpuChunkStart = numTasksPerGpu*numGpusPerLoc*here.id +
                            sublocId*numTasksPerGpu;
      coforall workerId in 0..<numTasksPerGpu {
        const myChunk = 1+(gpuChunkStart+workerId)*perTaskSize..#perTaskSize;

        var GpuVec = CpuVec[myChunk];
        GpuVec += 1;
        CpuVec[myChunk] = GpuVec;

      }
    }
  }
  writeln(CpuVec);
}

stopGpuDiagnostics();

const totalNumTasks = numLocales*numGpusPerLoc*numTasksPerGpu;
assertGpuDiags(kernel_launch=totalNumTasks,
               host_to_device=totalNumTasks,
               device_to_host=totalNumTasks);
