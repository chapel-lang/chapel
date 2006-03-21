config var n: int = 4;

var D: domain(1) = (1..n);

var A: [D] float;

forall i in D {
  A(i) = i;
}

fun foo(X: [D] float) {
  writeln("X is: ", X);
}


foo(A);
