proc foo(X: [?r1, ?r2] real) {
  writeln("Got 2D array with dimensions: ", (r1, r2));
}

proc foo(X: [?r1, ?r2, ?r3] real) {
  writeln("Got 3D array with dimensions: ", (r1, r2, r3));
}

var A: [1..3, 1..5] real;
var B: [1..3, 1..5, 1..7] real;

foo(A);
foo(B);
