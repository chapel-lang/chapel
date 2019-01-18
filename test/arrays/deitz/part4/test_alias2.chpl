var D = {1..5, 1..5};
var A: [D] int;

for (i,j) in D do
  A(i,j) = 4 + 5*i + j;

ref AA = A[2..4, 2..4];

AA(3,3) = 99;

writeln(A);
writeln(AA);
