config var hi: int(64) = 8;

var D: domain(1, int(64)) = {1..hi};
var A: [D] real = [i in D] i;

writeln("D is: ", D);
writeln("A is: ", A);

foo(A[1..8 by 2].reindex({1..4}));

proc foo(X:[1..4]) {
  writeln("X is: ", X);
  writeln("X(2) is: ", X(2));
}
