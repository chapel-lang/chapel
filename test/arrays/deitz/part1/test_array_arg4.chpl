config var n: int = 4;

var D: domain(1) = {1..n};
var A: [D] int;

proc foo(X: [D] int) {
  for i in D do
    writeln(X(i));
}

for i in D do
  A(i) = 2;

foo(A);
