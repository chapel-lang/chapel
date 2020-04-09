proc foo(X: [1..5] int) {
  for i in X.domain do
    writeln(i, ": ", X(i));
}

var A: [1..10] int;
A = 1..10;

foo(=>A[6..10]);
foo(=>A[4..8]);
foo(=>A[2..6]);
foo(=>A[1..5]);
