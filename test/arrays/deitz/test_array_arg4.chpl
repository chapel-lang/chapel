config var n: int = 4;

var D: domain(1) = [1..n, 1..n];
var A: [D] int;

def foo(X: [D] int) {
  for i in D do
    writeln(X(i));
}

for i in D do
  A(i) = 2;

foo(A);
