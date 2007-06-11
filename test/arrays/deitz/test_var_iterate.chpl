var A: [1..10] int;

A = 1..10;

writeln(A);

for e in A[2..9] do
  e = 0;

writeln(A);
