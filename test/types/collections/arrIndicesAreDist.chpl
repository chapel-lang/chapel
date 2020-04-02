use BlockDist;

var A = newBlockArr({1..10}, real);

forall i in A.indices do
  A[i] = here.id;

writeln(A);
