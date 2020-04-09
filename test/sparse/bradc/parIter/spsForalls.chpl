config const n = 9;

const D = {1..n, 1..n};

var S: sparse subdomain(D);

var A: [S] real;

for i in 1..n do
  S += (i,i);

forall (i,j) in S do
  A(i,j) = i + j/10.0;

writeln("A is: ", A);

forall a in A do
  a = -a;

writeln("A is: ", A);

