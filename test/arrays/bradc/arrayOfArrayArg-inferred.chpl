proc foo(X) {
  writeln("X is: ", X);
}

var A: [1..3] [1..2] real = ((1.1, 1.2), (2.1, 2.2), (3.1, 3.2));

foo(A);
