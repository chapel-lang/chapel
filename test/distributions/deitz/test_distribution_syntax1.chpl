use BlockDist;

config const n: int = 4;

var A: [{1..n,1..n} dmapped new blockDist(rank=2,boundingBox={1..n,1..n})] 2*int;

forall (i,j) in A.domain with (ref A) {
  A(i,j) = (here.id,(i-1)*n + j);
}
writeln(A);

var B: [{1..n, 1..n} dmapped new blockDist(rank=2,boundingBox={1..n,1..n})] 2*int;

forall (i,j) in B.domain with (ref B) {
  B(i,j) = (here.id,(i-1)*n + j);
}
writeln(B);
