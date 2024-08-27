// we want dynamic checks to fail here. The way to check is to make sure that
// `localAccess` is never called.
use BlockDist;

var A: [blockDist.createDomain({1..10})] int = 5;

var db = {1..5};
var B: [blockDist.createDomain(db)] int = 1;

forall (b, i) in zip(B, db) {
  b = A[i];
}

writeln(B);
