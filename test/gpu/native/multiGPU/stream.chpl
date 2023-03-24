use Time;
use Math;

config type elemType = real(64);

config const usePromotion = false;
config const alpha = 1.4;
config const iters = 10;
config const validate = false;
config const printOutput = false;
config const reportOverall = false;

config const perGpuVecSize = 100;
const numGpus = here.gpus.size;
assert(numGpus <= here.gpus.size);

const numElems = numGpus*perGpuVecSize;

const ProblemSpace = {0..#numGpus*perGpuVecSize};

// noinit and local for faster initialization
var A, B, C: [ProblemSpace] real = noinit;
local {
  B = 1;
  C = 2;
}

record times {
  var hostToDev: real;
  var devToHost: real;
  var kernel: real;
}

var overallTimer: stopwatch;

var gpuTimes: [here.gpus.domain] times;
overallTimer.start();
coforall (gpu, id) in zip(here.gpus, here.gpus.domain) do on gpu {
  const GpuSpace = {id*perGpuVecSize..#perGpuVecSize};
  var GpuA: [GpuSpace] elemType;
  var gpuTimer: stopwatch;

  gpuTimer.start();
  var GpuB = B[GpuSpace];
  var GpuC = C[GpuSpace];
  gpuTimer.stop();
  gpuTimes[id].hostToDev = gpuTimer.elapsed();
  gpuTimer.clear();


  for curIter in 0..iters {
    if curIter == 1 then gpuTimer.start();
    if usePromotion {
      GpuA = GpuB + alpha * GpuC;
    }
    else {
      foreach (a, b, c) in zip(GpuA, GpuB, GpuC) do
        a = b + alpha * c;
    }
  }
  gpuTimer.stop();
  gpuTimes[id].kernel = gpuTimer.elapsed()/iters;
  gpuTimer.clear();

  gpuTimer.start();
  A[GpuSpace] = GpuA;
  gpuTimer.stop();
  gpuTimes[id].devToHost = gpuTimer.elapsed();
  gpuTimer.clear();
}
overallTimer.stop();

if printOutput then writeln(A);

if validate {
  writeln((+ reduce A));
  writeln(numElems*(1+alpha*2));
  assert(isclose((+ reduce A), numElems*(1+alpha*2)));
}

for gpuId in here.gpus.domain {
  write("GPU ", gpuId);

  const ref t = gpuTimes[gpuId];
  writeln("\t", t);

  const perVectorNumBytes = perGpuVecSize*numBytes(elemType);
  writeln("\tHost to Dev BW (GB/s): ", ((perVectorNumBytes*2)/t.hostToDev)*1e-9);
  writeln("\tDev to Host BW (GB/s): ", (perVectorNumBytes/t.devToHost)*1e-9);
  writeln("\tKernel throughput (GB/s): ", (3*perVectorNumBytes/t.kernel)*1e-9);
}

if reportOverall {
  if iters != 1 then writeln("Warning: We ran stream multiple times!");
  writeln("Overall throughput (GB/s): ",
          numElems*3*numBytes(elemType)/overallTimer.elapsed()*1e-9);
}
