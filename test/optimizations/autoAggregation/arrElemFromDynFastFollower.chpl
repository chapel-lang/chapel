use BlockDist;

var A = newBlockArr(0..10, int);
var B = newBlockArr(0..10, int);
var someOther = newBlockArr(0..10, int);

// B is a dynamic fast follower, so we should be able to see `b` as local within
// the fast follower body, and source-aggregate
forall (a,b) in zip(A,B) {
  b = someOther[a];
}

writeln(B);

// same idea, but for destination aggregation
forall (a,b) in zip(A,B) {
  someOther[a] = b;
}

writeln(B);

