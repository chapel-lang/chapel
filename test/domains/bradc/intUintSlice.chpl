var D = {1..10};
var DU = {1..10:uint};

var A: [1..10] real;

forall i in D[(...DU.dims())] with (ref A) do
  A[i] += 1;

writeln("A is: ", A);

forall i in DU[(...D.dims())] with (ref A) do
  A[i:int] += 1;

writeln("A is: ", A);

forall i in D[DU] with (ref A) do
  A[i] += 1;

writeln("A is: ", A);

forall i in DU[D] with (ref A) do
  A[i:int] += 1;

writeln("A is: ", A);
