config const n = 1024;

writeln("Number of sublocales: ", here.gpus.size);

var A: [0..#n] int;
// assign half the work to CPU, the rest to GPUs. Assume divisibility
const numGPUs = here.gpus.size;
const cpuSize = n/2; 
const gpuSize = (n/2)/numGPUs;

assert(n%2 == 0);
assert((n/2)%numGPUs == 0);

cobegin {
  A[0..#cpuSize] += 1;
  
  coforall subloc in 0..<numGPUs do on here.gpus[subloc] {
    const myShare = cpuSize+gpuSize*subloc..#gpuSize;
    
    var AonThisGPU = A[myShare];
    AonThisGPU += 1;
    A[myShare] = AonThisGPU;
  }
}

assert((+ reduce A) == n);
