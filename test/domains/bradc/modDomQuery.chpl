config var n = 9;

var D: domain(1) = {1..n};

var A: [D] real = [i in D] i;

writeln("A is: ", A);

foo(A);

writeln("A is: ", A);

proc foo(X: [?D]) {
  D = {1..2*n};
}

