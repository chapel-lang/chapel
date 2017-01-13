const D = {1..9, 1..9};
var A: [D] real;

forall (i,j) in D do
  A[i,j] = i + j/10.0;

A[5..6, 7..8] = 0.0;

writeln(A);
