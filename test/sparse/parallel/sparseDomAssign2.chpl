config const n = 9;

const D = {1..n, 1..n};

var S1, S2: sparse subdomain(D);

var A1: [S1] real;

for i in 1..n do
  S1 += (i,i);

writeln("S1 is:\n", S1);

for i in 1..n do
  S2 += (i, n+1-i);

writeln("S2 is:\n", S2);

forall (i,j) in S1 do
  A1(i,j) = i + j/10.0;

writeln("A1 = \n", A1);

// Test array-preserving assignment: (n/2, n/2) should be preserved
S1 = S2;

writeln("A1 = \n", A1);
