config var n: int = 8;

var D: domain(1) = [1..n];

var A: [D] float;

for i in D {
  A(i) = -i:float;
}

// writeln("A is: ", A);

var maxA = max reduce A;

writeln("maxA is: ", maxA);

