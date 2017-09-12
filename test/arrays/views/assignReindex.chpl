const D = {1..9, 1..9};
var A: [D] real;

forall (i,j) in D do
  A[i,j] = i + j/10.0;

A.reindex(0..8, 2..10) = 0.0;

writeln(A, "\n");

forall a in A.reindex(0..8, 2..10) do
  a = 1.0;

writeln(A, "\n");

ref B = A.reindex(2..10, 0..8);

forall (i,j) in B.domain do
  B[i,j] = i + j/10.0;

writeln(A, "\n");
