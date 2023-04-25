var D = {1..4} by 2;

var A: [D] real;

for i in D do
  A[i] = i / 10.0;

writeln("head/tail of A: ", (A.head(), A.tail()));
