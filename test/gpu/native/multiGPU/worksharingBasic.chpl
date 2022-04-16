config const n = 1024;

writeln("Number of sublocales: ", here.getChildCount());

var A: [0..#n] int;
// assign half the work to CPU, the rest to GPUs. Assume divisibility
const numGPUs = here.getChildCount()-1;
const cpuSize = n/2; 
const gpuSize = (n/2)/numGPUs;

assert(n%2 == 0);
assert((n/2)%numGPUs == 0);

cobegin {
  A[0..#cpuSize] += 1;
  
  coforall subloc in 1..numGPUs do on here.getChild(subloc) {
    const myShare = cpuSize+gpuSize*(subloc-1)..#gpuSize;
    
    var AonThisGPU = A[myShare];
    AonThisGPU += 1;
    A[myShare] = AonThisGPU;
  }
}

assert((+ reduce A) == n);
