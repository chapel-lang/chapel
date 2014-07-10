//
// privateSpace.chpl: This example demonstrates the use of PrivateSpace to
// create an array with a single element per locale.  Using comm diagnostics,
// we'll see how to compute each locale's local sum of all the elements in a
// Block-dmapped array on every locale without communication.
//
use BlockDist, PrivateDist;

config var n: int = 8;

var P: [PrivateSpace] atomic int;
//var D: domain(1) dmapped new dmap(new Block(boundingBox={1..n})) = {1..n};
//var A: [D] int;

//startCommDiagnostics();
coforall loc in Locales do on loc {
  P(loc.id).write(100+loc.id);
}
//stopCommDiagnostics();

writeln(P);
P.write(0);
writeln(P);
