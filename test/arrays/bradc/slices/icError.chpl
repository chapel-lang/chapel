config const n = 4;

const D = {1..n, 1..n};

var A: [D] real = [(i,j) in D] (i-1)*n + j;

writeln("A was:\n", A);

foo(A[1, 1..4], A[4, 1..4]);

writeln("A is:\n", A);


proc foo(X: [?D1], Y: [?D2]) {
  writeln("X is:\n", X);
  writeln("Y is:\n", Y);
}
