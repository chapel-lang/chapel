const D = {1..9, 1..9};
var A: [D] real;

forall (i,j) in D do
  A[i,j] = i + j/10.0;

A[5..6, 7..8] = 0.0;

writeln(A, "\n");

forall a in A[5..6, 7..8] do
  a = 1.0;

writeln(A, "\n");

forall a in A[5..6, 7..8] do
  a = 2.0;

writeln(A, "\n");
