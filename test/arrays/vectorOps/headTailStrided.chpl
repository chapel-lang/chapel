var D = {1..4} by 2;
var D2 = {1..4} by -2;

var A: [D] real;
var A2: [D2] real;

for i in D do
  A[i] = i / 10.0;

for i in D2 do
  A2[i] = i / 10.0;

writeln("head/tail of A: ", (A.head(), A.tail()));
writeln("head/tail of A2: ", (A2.head(), A2.tail()));
