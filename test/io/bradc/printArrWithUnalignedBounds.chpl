const D = {1..6, 1..6};
const D2 = {1..6 by 3 align 2, 1..6 by 3 align 2};

writeln("D2 is: ", D2);

var A: [D] real;
var A2: [D2] real;

forall (i,j) in D do
  A[i,j] = i + j/10.0;

writeln("A is:\n", A);
writeln("----");

forall (i,j) in D2 do
  A2[i,j] = A[i,j];

writeln("A2 is:\n", A2);
writeln("----");
