import RangeChunk.chunks;

config const n = 32;         // now, our application has `--n` to set this!
config const sliceSize = 4;  // number of elements per slice

var HostArr: [1..n] int;  // allocated on the host
HostArr = 1;              // executes on [multicore] CPU

coforall (loc, locChunk) in zip(Locales, chunks(1..n, numLocales)) {
  on loc {
    const numGpus = here.gpus.size;
    coforall (gpu, gpuChunk) in zip(here.gpus, chunks(locChunk, numGpus)) {
      on gpu {
        const numSlices = gpuChunk.size/sliceSize;  // assume divisibility

        coforall chunk in chunks(gpuChunk, numSlices) {
          var DevArr: [chunk] int;  // allocated per device

          DevArr = HostArr[chunk];  // copy a slice from host to device
          DevArr += 1;              // executes on GPU as a kernel
          HostArr[chunk] = DevArr;  // copy from device to a slice on host
        }
      }
    }
  }
}

writeln(HostArr);  // prints "2 2 2 2 2 ..."
