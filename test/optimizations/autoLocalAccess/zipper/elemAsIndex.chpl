use BlockDist;

var A = Block.createArray({1..10}, int);

for (a,i) in zip(A, A.domain) do a=i;

forall (a,i) in zip(A, 1..) {
  A[a] = a*i;
}

writeln(A);
