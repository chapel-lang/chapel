//
// privateSpace.chpl: This example demonstrates the use of PrivateSpace to
// create an array with a single element per locale.  Using comm diagnostics,
// we'll see how to compute each locale's local sum of all the elements in a
// Block-distributed array on every locale without communication.
//
use BlockDist, PrivateDist;

config var n: int = 8;

var P: [PrivateSpace] int;
var D: domain(1) distributed distributionValue(new Block(1,int,bbox=[1..n])) = [1..n];
var A: [D] int;

startCommDiagnostics();
coforall loc in Locales do on loc {
  P(loc.id) = loc.id;
}
stopCommDiagnostics();

writeln(P);
writeln("diagnostics = ", getCommDiagnostics());

P = 0;
A = 1..n;

startCommDiagnostics();
forall a in A do
  P(here.id) += a;
stopCommDiagnostics();

writeln(A);
writeln(P);
writeln("diagnostics = ", getCommDiagnostics());

forall p in P {
  writeln(here.id, ": ", p);
}
