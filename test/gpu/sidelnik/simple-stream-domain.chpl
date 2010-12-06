use GPUDist;

config const threadsPerBlock = 4;

var m = 64;

def main() {

var alpha = 3.0;

const GPUBlockDist = new dmap(new GPUDist(rank=1, tbSizeX=threadsPerBlock));

const space: domain(1) dmapped GPUBlockDist = [1..m];

var A, B, C : [space] real;

A = 4.0;
B = 9.0;
C = 4.0;

forall i in space do
  A(i) = B(i) + alpha * C(i);

writeln(A);
}
