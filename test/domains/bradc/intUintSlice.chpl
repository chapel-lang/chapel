var D = {1..10};
var DU = {1..10:uint};

var A: [1..10] real;

forall i in D[(...DU.dims())] do
  A[i] += 1;

writeln("A is: ", A);

forall i in DU[(...D.dims())] do
  A[i:int] += 1;

writeln("A is: ", A);

forall i in D[DU] do
  A[i] += 1;

writeln("A is: ", A);

forall i in DU[D] do
  A[i:int] += 1;

writeln("A is: ", A);
