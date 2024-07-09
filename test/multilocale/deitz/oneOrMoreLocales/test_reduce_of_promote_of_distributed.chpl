use BlockDist;

config const n: int(64) = 4;

const MyBlock = new blockDist(rank=2, idxType=int(64), boundingBox={1..n,1..n});
const D: domain(2,int(64)) dmapped MyBlock = {1..n,1..n};
var A, B: [D] int(64);

forall (i,j) in D with (ref A) do A(i,j) = i+j;
forall (i,j) in D with (ref B) do B(i,j) = i*j;

writeln(+ reduce (A+B));
