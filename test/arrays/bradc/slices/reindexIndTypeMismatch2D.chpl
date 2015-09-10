config var hi: int(64) = 8;

var D: domain(2, int(64)) = {1..hi, 1..hi};
var A: [D] real = [(i,j) in D] (i-1)*hi + j;

writeln("D is: ", D);
writeln("A is: ", A);

foo(A[1..8 by 2, 1..8 by 2].reindex({1..4, 1..4}));

proc foo(X:[1..4, 1..4]) {
  writeln("X is: ", X);
  writeln("X(2,3) is: ", X(2,3));
}
