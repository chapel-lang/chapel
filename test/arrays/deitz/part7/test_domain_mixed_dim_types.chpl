var D = {1..6:uint, 1..7};

var A: [D] uint;

for (i,j) in D do
  A(i,j) = i + j;

writeln(A);
