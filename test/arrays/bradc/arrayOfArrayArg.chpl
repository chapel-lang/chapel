proc foo(X: [1..3] [1..2] real) {
  writeln("X is: ", X);
}

proc bar(X: [1..3] [] real) {
  writeln("X is: ", X);
}

proc baz(X: [] [1..2] real) {
  writeln("X is: ", X);
}

proc bak(X: [] [] real) {
  writeln("X is: ", X);
}

var A: [1..3] [1..2] real = ((1.1, 1.2), (2.1, 2.2), (3.1, 3.2));

foo(A);
bar(A);
baz(A);
bak(A);
