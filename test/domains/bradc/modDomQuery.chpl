config var n = 9;

var D: domain(1) = [1..n];

var A: [i in D] real = i;

writeln("A is: ", A);

foo(A);

writeln("A is: ", A);

def foo(X: [?D]) {
  D = [1..2*n];
}

