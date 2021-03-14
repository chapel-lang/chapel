use driver_domains;

writeln(Dom2D(10..30, 2));
for i in Dom3D(2..4, 4..6, 1) do
  writeln(i);

var D = Dom2D(-5..5, 5);
var A: [D] int;

for e in A do e = next();
writeln(A);
