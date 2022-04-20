use Time;

writeln("Number of sublocales: ", here.getChildCount());

config const validate = true;
config const printStats = false;
config const debug = false;
config const justCPU = false;
config const numIters = 100;

config const alpha = 2;
config const n = 10;

config const cpuToGpuRatio = 2.0;

var t = new Timer();

var A, B, C: [0..#n] int;

B = 1;
C = 2;

var minTime = max(real);

for i in 1..numIters {
  t.start();
  if justCPU {
    A = B + alpha * C;
  }
  else {
    const numPUs = here.getChildCount(); 
    const numGPUs = numPUs-1;
    const chunkDiv = numGPUs+cpuToGpuRatio;
    var chunkSize = (n/chunkDiv):int;
    var cpuSize = (chunkSize*cpuToGpuRatio):int;
    cpuSize += (n-cpuSize)%numGPUs;
    var gpuChunkSize = (n-cpuSize)/numGPUs;
    const cpuRange = 0..#cpuSize;

    cobegin {
      A[cpuRange] = B[cpuRange] + alpha * C[cpuRange];

      coforall sublocID in 1..#numGPUs do on here.getChild(sublocID) {
        const myChunk = cpuSize+(sublocID-1)*gpuChunkSize..#gpuChunkSize;
        if debug then writeln(sublocID, ": ", myChunk);

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


if validate then
  assert(n*(1+alpha*2) == + reduce A);

if printStats {
  writeln("Performance (GB/s) = ", 3* numBytes(int) * n * 1e-9 / minTime );
}
