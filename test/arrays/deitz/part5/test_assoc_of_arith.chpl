var D: domain(int);
D += 2;
D += 5;
var A, B: [D] [1..3] int;

for i in D do
  for j in 1..3 do
    A(i)(j) = i + j;

writeln(A[2], " ", A[5]);

B = A;

writeln(B[2], " ", B[5]);
