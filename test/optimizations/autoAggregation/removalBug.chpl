// This is a funny case that came up while trying to test something else. We
// generate an aggregator, use it in both the fast and the slow follower bodies.
// Then, one ends up using aggregation, one ends up removing it. So, while
// removing the aggregation code from the rejecting block, we need to make sure
// not to remove too much.
use BlockDist;

type elemType = 2*int;

var A = newBlockArr(1..10, 1..10, elemType);

forall a in A {
  a = A[1,1];
}

writeln(A);
