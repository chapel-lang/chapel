config var n : int = 7;

var D : domain(2) = {1..n, 1..n};

var A : [D] real;

forall k in D {
  A(k) = 1;
}

writeln("A:\n", A);
