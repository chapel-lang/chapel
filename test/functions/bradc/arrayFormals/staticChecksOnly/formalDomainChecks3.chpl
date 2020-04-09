proc foo(X: [1..3] real) {
  writeln("In foo!");
}

var A: [1..3, 1..3] real;

foo(A);
