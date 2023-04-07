config const nPerGpu = 100;

const n = numLocales * here.gpus.size * nPerGpu;

var CpuVec: [1..n] int = 5;

proc sublocaleId { return chpl_task_getRequestedSubloc(); }

coforall loc in Locales do on loc {
  coforall gpu in here.gpus do on gpu {
    const myChunk = sublocaleId*nPerGpu..#nPerGpu;

    var GpuVec = CpuVec[myChunk];
    GpuVec += 1;
    CpuVec[myChunk] = GpuVec;

  }
}
writeln(CpuVec);

