var D = {1..8:uint(64)};
var A: [D] uint(64);

for i in D do
  A(i) = i;

writeln(A);
