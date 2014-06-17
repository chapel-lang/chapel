enum probClass {S, W, A, B, C};

var PD: domain(probClass);

var A: [PD] real;

for i in PD {
  A(i) = i:real;
}

for a in A {
  writeln("a is: ", a);
}
