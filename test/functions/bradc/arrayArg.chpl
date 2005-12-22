config var n: integer = 4;

var D: domain(1) = [1..n];

var A: [D] float;

forall i in D {
  A(i) = i;
}

function foo(X) {
  writeln("X is: ", X);
}


foo(A);
