var D = {1..10};

var A: [D] real;



proc foo(X = A) {
  writeln("X is: ", X);
  X += 0.1;
  writeln("X is: ", X);
}

writeln("A is: ", A);
foo(A);
writeln("A is: ", A);
foo();
writeln("A is: ", A);

