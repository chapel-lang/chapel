config var hi: int(64) = 8;

var D: domain(1, int(64)) = [1..hi];
var A: [i in D] real = i;

writeln("D is: ", D);
writeln("A is: ", A);

foo(A[1..8 by 2]);

def foo(X:[1..4]) {
  writeln("X is: ", X);
  writeln("X(2) is: ", X(2));
}
