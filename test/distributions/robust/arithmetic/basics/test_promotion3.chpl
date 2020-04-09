use driver;

const Dom: domain(2) dmapped Dist2D = {1..4, 1..4};

var A, B: [Dom] int;

for e in A do e = next();
for e in B do e = next();

proc foo(a: int, b: int) return a + b;

var C = foo[A, B];

writeln(C);
