var O: domain(opaque);

var A: [O] real;

var newInd = O.create();

A(newInd) = 1.2;

for a in A {
  writeln("a is: ", a);
}
