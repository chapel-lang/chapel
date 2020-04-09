// Test errors in leader-follower behavior of arrays & tuples zippered

var A: [1..2, 1..2] int;
var V = (1,2,3,4);

// zip(array, tuple)
forall (a, v) in zip(A, V) {
  a = v;
}

