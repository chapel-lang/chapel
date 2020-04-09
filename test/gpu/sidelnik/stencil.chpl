use GPUDist, Random;

config const threadsPerBlock = 16;
config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.currentTime else 314159265;

var m = 64: int(64);

proc main() {

const GPUBlockDist = new GPUDist(rank=1, threadsPerBlock=threadsPerBlock);

const space: domain(1, int(64)) dmapped GPUBlockDist = [1..m];
const subspace: domain(1, int(64)) dmapped GPUBlockDist = [2..m-1];

var gpuA, gpuB : [space] real;

var hostA, hostB : [1..m] real;

var randlist = new RandomStream(seed);
randlist.fillRandom(hostB);
writeln(hostB);

gpuB = hostB;

forall i in subspace do {
  gpuA(i) = (gpuB(i-1) + gpuB(i) + gpuB(i+1))/3;
}

hostA = gpuA;

writeln(hostA);
}
