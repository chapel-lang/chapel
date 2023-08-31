use Time;
use Math;
use ChplConfig;

config type elemType = real(64);

config const usePromotion = false;
config const alpha = 1.4;
config const iters = 10;
config const validate = false;
config const printOutput = false;
config const reportIndividual = true;
config const reportAggregate = true;
config const SI = true;

config const perGpuVecSize = 100;
const numGpus = here.gpus.size;
assert(numGpus <= here.gpus.size);

const numElems = numGpus*perGpuVecSize;

const ProblemSpace = {0..#numGpus*perGpuVecSize};

// noinit and local for faster initialization
var A, B, C: [ProblemSpace] real = noinit;

// our single-GPU basic Stream avoids using `fillRandom` because it's behavior
// is uncertain on the GPU. Here, we are creating the input on host, so it
// should be technically safe to use `fillRandom` here, but I am avoiding it to
// be consistent. See https://github.com/chapel-lang/chapel/issues/22708 for
// `fillRandom` on GPUs.
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

coforall (gpu, id) in zip(here.gpus, here.gpus.domain) with (ref gpuTimes) do on gpu {
  const GpuSpace = {id*perGpuVecSize..#perGpuVecSize};
  var GpuA: [GpuSpace] elemType;
  var gpuTimer: stopwatch;

  gpuTimer.start();
  var GpuB = B[GpuSpace];
  var GpuC = C[GpuSpace];
  gpuTimer.stop();
  gpuTimes[id].hostToDev = gpuTimer.elapsed();
  gpuTimer.clear();


  // curIter == 0 is a warm up iteration that's not timed
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
  var result = 0.0;
  if CHPL_GPU=="amd" {
    // reductions with AMD+AOD hangs see
    // https://github.com/chapel-lang/chapel/issues/22736
    for a in A do result += a;
  }
  else {
    result = + reduce A;
  }
  const expected = numElems*(1+alpha*2);

  assert(isClose(result, expected));
}

const tputFactor = if SI then 1e9 else 1<<30;
const tputUnit = if SI then "GB/s" else "GiB/s";

var aggThroughput = 0.0;
for gpuId in here.gpus.domain {
  const ref t = gpuTimes[gpuId];
  const perVectorNumBytes = perGpuVecSize*numBytes(elemType);
  const throughput = (3*perVectorNumBytes/t.kernel)/tputFactor;
  aggThroughput += throughput;

  if reportIndividual {
    write("GPU ", gpuId);
    writeln("\t", t);

    writef("\tHost to Dev BW (%s): %r\n", tputUnit,
           ((perVectorNumBytes*2)/t.hostToDev)/tputFactor);
    writef("\tDev to Host BW (%s): %r\n", tputUnit,
            (perVectorNumBytes/t.devToHost)/tputFactor);
    writef("\tKernel throughput (%s): %r\n", tputUnit, throughput);
  }
}

if reportAggregate {
  writef("Aggregate performance (%s): %r\n", tputUnit, aggThroughput);
}
