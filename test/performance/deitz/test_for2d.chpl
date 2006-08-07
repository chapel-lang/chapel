var D = [1..128, 1..128];
var A: [D] float;

for i,j in D do
  A(i,j) = i + j;

writeln(A(52, 52));
