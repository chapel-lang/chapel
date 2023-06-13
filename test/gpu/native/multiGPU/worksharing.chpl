use Time;
use GpuDiagnostics;
use MemDiagnostics;

config const validate = true;
config const printStats = false;
config const debug = false;
config const justCPU = false;
config const numIters = 100;

config const alpha = 10;
config const n = 10;

config const cpuToGpuRatio = 2.0;

var t = new stopwatch();

var A, B, C: [0..#n] int;

B = 1;
C = 2;

var minTime = max(real);

startGpuDiagnostics();

for i in 1..numIters {
  t.start();
  if justCPU {
    A = B + alpha * C;
  }
  else {
    const numGPUs = here.gpus.size;
    const chunkDiv = numGPUs+cpuToGpuRatio;
    var chunkSize = (n/chunkDiv):int;
    var cpuSize = (chunkSize*cpuToGpuRatio):int;
    cpuSize += (n-cpuSize)%numGPUs;
    var gpuChunkSize = (n-cpuSize)/numGPUs;
    const cpuRange = 0..#cpuSize;

    cobegin {
      A[cpuRange] = B[cpuRange] + alpha * C[cpuRange];

      coforall (gpu, gpuID) in zip(here.gpus, here.gpus.domain) do on gpu {
        const myChunk = cpuSize+gpuID*gpuChunkSize..#gpuChunkSize;
        if debug then writeln(gpuID, ": ", myChunk);

        var Aloc: [myChunk] int;
        var Bloc = B[myChunk];
        var Cloc = C[myChunk];

        Aloc = Bloc + alpha * Cloc;

        A[myChunk] = Aloc;
      }
    }
  }
  t.stop();
  if t.elapsed() < minTime then minTime = t.elapsed();
}

stopGpuDiagnostics();
const nLaunch = getGpuDiagnostics().kernel_launch;

assert(nLaunch == here.gpus.size*numIters);

writeln(A);

if validate then
  assert(n*(1+alpha*2) == + reduce A);

if printStats {
  writeln("Performance (GB/s) = ", 3* numBytes(int) * n * 1e-9 / minTime );
}
