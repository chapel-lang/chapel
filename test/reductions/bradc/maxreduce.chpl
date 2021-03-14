config var n: int = 8;

var D: domain(1) = {1..n};

var A: [D] real;

for i in D {
  A(i) = -i:real;
}

// writeln("A is: ", A);

var maxA = max reduce A;

writeln("maxA is: ", maxA);

