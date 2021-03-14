// Test leader-follower behavior of arrays & tuples zippered

var A = [0,0,0];
var V = (1,2,3);

// zip(array, tuple)
forall (a, v) in zip(A, V) {
  a = v;
}
writeln(A);

A = 0;

// zip(tuple, array)
forall (v, a) in zip(V, A) {
  a = v;
}

writeln(A);

// zip(tuple, tuple)
var s: int;
forall (x, y) in zip(V, V) with (+ reduce s) {
  s += x*y;
}

writeln(s);
