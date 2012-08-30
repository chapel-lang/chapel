config var n: int = 4;

var D: domain(1) = {1..n};

var A: [D] real;

forall i in D {
  A(i) = i;
}

proc foo(X) {
  writeln("X is: ", X);
}


foo(A);
