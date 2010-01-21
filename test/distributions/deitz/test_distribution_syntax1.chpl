use BlockDist;

config const n: int = 4;

var A: [[1..n,1..n] distributed new dist(new Block(rank=2,bbox=[1..n,1..n]))] 2*int;

forall (i,j) in A.domain {
  A(i,j) = (here.id,(i-1)*n + j);
}
writeln(A);

var B: [[1..n, 1..n] distributed Block(rank=2,bbox=[1..n,1..n])] 2*int;

forall (i,j) in B.domain {
  B(i,j) = (here.id,(i-1)*n + j);
}
writeln(B);
