use driver_domains;

writeln(Dom1D(-1..20));
writeln(Dom2D(10..30, -1..10));
for i in Dom3D(2..4, 4..6, 6..8) do
  writeln(i);
for i in Dom3D(2..4, 4..6, 0..8) do
  writeln(i);

var D = Dom2D(-5..5, -5..5);
var A: [D] int;

for e in A do e = next();
writeln(A);
writeln(dist_eq(A.domain.dist, Dist2D));
