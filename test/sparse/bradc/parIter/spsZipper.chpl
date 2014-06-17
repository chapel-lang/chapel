config const n = 9;

const D = {1..n, 1..n};

var S: sparse subdomain(D);

var A, A2: [S] real;

for i in 1..n do
  S += (i,i);

forall (a, (i,j)) in zip(A, S) do
  a = i + j/10.0;

writeln("A is: ", A);

forall ((i,j), a) in zip(S, A2) do
  a = -(i + j/10.0);

writeln("A2 is: ", A2);

writeln("Zippering, A and A2 are: ");
forall (a, a2) in zip(A, A2) do
  writeln((a,a2));
