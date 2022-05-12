use GPUDiagnostics;

config const n = 32;

var A: [0..#n] int;
// assign half the work to CPU, the rest to GPUs. Assume divisibility
const numGPUs = here.gpus.size;
const cpuSize = n/2;
const gpuSize = (n/2)/numGPUs;

assert(n%2 == 0);
assert((n/2)%numGPUs == 0);

startGPUDiagnostics();

cobegin {
  A[0..#cpuSize] += 1;

  coforall (gpu, gpuID) in zip(here.gpus, here.gpus.domain) do on gpu {
    const myShare = cpuSize+gpuSize*gpuID..#gpuSize;

    var AonThisGPU = A[myShare];
    AonThisGPU += 1;
    A[myShare] = AonThisGPU;
  }
}

stopGPUDiagnostics();

writeln(A);

const nLaunch = getGPUDiagnostics().kernel_launch;

assert(nLaunch == here.getChildCount());
assert((+ reduce A) == n);
