use GpuDiagnostics;

config const n = 20;
config const tSize = 5; // size per each iteration done by each task
config const numWorkers = 2;

assert(n%tSize == 0);
assert(numWorkers >= 1);

const numChunks = n/tSize;

var HostIn, HostOut: [0..#n] real;
HostIn = 1;

proc kernel(InArr, ref OutArr) {
  @assertOnGpu
  foreach (i,o) in zip(InArr, OutArr) do o = i*2;
}

var curChunk: atomic int = 0;

startGpuDiagnostics();
{ // slide starts here
  on here.gpus[0] {
    coforall worker in 0..#numWorkers {
      var DevIn, DevOut: [0..#tSize] real;
      while true {
        // dynamically pick the next chunk
        const myChunkId = curChunk.fetchAdd(1);
        if myChunkId >= numChunks then break;

        const myChunk = myChunkId*tSize..#tSize;

        DevIn = HostIn[myChunk];   // copy in
        kernel(DevIn, DevOut);     // kernel
        HostOut[myChunk] = DevOut; // copy out
      }
    }
  }
}
stopGpuDiagnostics();

writeln(HostOut);

assertGpuDiags(kernel_launch_aod=8, kernel_launch_um=4);
