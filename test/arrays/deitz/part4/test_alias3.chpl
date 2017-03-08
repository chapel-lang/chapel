var D = {1..5, 1..5};
var A: [D] int;

for (i,j) in D do
  A(i,j) = 4 + 5*i + j;

ref AA: [6..10,6..10] = A;

AA(8,8) = 99;

writeln(A);
writeln(AA);
