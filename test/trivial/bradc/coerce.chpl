config var n: int = 8;

var D: domain(1) = {1..n};

var A: [D] real;

for i in D {
  A(i) = i;
}

writeln("A is: ", A);
