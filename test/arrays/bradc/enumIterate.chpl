enum probClass {S, W, A, B, C};

var PD: domain(probClass) = probClass.S..probClass.C;

var A: [PD] real;

for (i, a) in zip(PD, A) {
  a = i:real;
}

for e in probClass {
  writeln("a is: ", A[e]);
}
