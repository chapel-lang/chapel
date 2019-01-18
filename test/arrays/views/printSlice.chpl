const D = {1..10, 1..10};
var A: [D] real;

forall (i,j) in D do
  A[i,j] = i + j/10.0;

writeln(A[5..6, 7..8]);
