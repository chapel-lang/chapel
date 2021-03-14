use driver_domains;

var A, B, C: [Dom2D] real;

for e in A do e = next();
for e in B do e = next();

proc foo(x: real, y: real) {
  return sqrt(x**2 + y**2);
}

C = foo(A, B);

writeln(C);
