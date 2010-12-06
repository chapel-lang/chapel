use GPUDist;

config const threadsPerBlock = 16;

var m = 64;

def main() {

const GPUBlockDist = new dmap(new GPUDist(rank=1, tbSizeX=threadsPerBlock));

const space: domain(1) dmapped GPUBlockDist = [1..m];
const subspace: domain(1) dmapped GPUBlockDist = [2..m-1];

var gpuA, gpuB : [space] real;

forall i in space do
  gpuB(i) = i : real;

writeln(gpuB);

forall i in subspace do {
  gpuA(i) = (gpuB(i-1) + gpuB(i) + gpuB(i+1))/3;
}

writeln(gpuA);
}
