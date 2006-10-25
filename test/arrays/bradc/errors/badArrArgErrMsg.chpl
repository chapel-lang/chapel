config var N = 16;

var D = [1..N];
var A: [D] complex;

foo(A);

def foo(X: [?Dlocal] real) {
  writeln("X is: ", X);
}
