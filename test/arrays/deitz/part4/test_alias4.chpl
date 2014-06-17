var D = {1..5, 1..5};
var A: [D] int;

for (i,j) in D do
  A(i,j) = 4 + 5*i + j;

var AA: [7..9,7..9] => A[2..4,2..4];

AA(8,8) = 99;

writeln(A);
writeln(AA);
