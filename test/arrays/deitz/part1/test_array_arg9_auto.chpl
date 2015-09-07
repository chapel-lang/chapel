proc foo(X: [1..5] int) {
  for i in X.domain do
    X(i) = i;
}

var D = {1..10};
var D1 = {6..10};
var D2 = {3..7};
var D3 = {1..5};
var A: [D] int;

A = 1..10;
writeln(A);
foo(=>A[D1]);
writeln(A);
foo(=>A[D2]);
writeln(A);
foo(=>A[D3]);
writeln(A);
