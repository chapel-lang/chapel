use Time;
use Math;

config type elemType = real(64);

config const usePromotion = false;
config const alpha = 1.4;
config const iters = 10;
config const validate = false;
config const printOutput = false;
config const reportIndividual = true;
config const reportAggregate = true;

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

var gpuTimes: [here.gpus.domain] times;

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

if printOutput then writeln(A);

if validate {
  const result = + reduce A;
  const expected = numElems*(1+alpha*2);

  assert(isclose(result, expected));
}

var aggThroughput = 0.0;
for gpuId in here.gpus.domain {
  const ref t = gpuTimes[gpuId];
  const perVectorNumBytes = perGpuVecSize*numBytes(elemType);
  const throughput = (3*perVectorNumBytes/t.kernel)*1e-9;
  aggThroughput += throughput;

  if reportIndividual {
    write("GPU ", gpuId);
    writeln("\t", t);

    writeln("\tHost to Dev BW (GB/s): ", ((perVectorNumBytes*2)/t.hostToDev)*1e-9);
    writeln("\tDev to Host BW (GB/s): ", (perVectorNumBytes/t.devToHost)*1e-9);
    writeln("\tKernel throughput (GB/s): ", throughput);
  }
}

if reportAggregate {
  writeln("Aggregate performance (GB/s): ", aggThroughput);
}
