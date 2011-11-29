use driver_domains;

writeln(Dom1D.translate(-1));
writeln(Dom1D.translate(1));

writeln(Dom1D.translate(-2));
writeln(Dom1D.translate(2));

writeln(Dom2D.translate(2, 2));
writeln(Dom3D.translate(-1, -2, -3));

var A: [Dom2D.translate(1, 1)] int;

writeln(A);
for i in A.domain do
  writeln(i);
writeln(dist_eq(A.domain.dist, Dist2D));
