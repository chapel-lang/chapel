use BlockDist;

config type elemType = int;

var A = Block.createArray(0..10, elemType);

forall (a, i) in zip(A, A.domain) {
  a = A[10-i];
}
