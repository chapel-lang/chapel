// Test errors in leader-follower behavior of arrays & tuples zippered

var D: domain(int);
var A: [D] int;
A[1] = 1;
A[2] = 2;
A[3] = 3;
var V = (1,2,3);

// zip(array, tuple)
forall (a, v) in zip(A, V) {
  a = v;
}

