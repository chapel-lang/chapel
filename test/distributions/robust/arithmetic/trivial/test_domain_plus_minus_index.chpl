use driver_domains;

writeln(Dom1D - 1);
writeln(Dom1D + 1);

writeln(Dom1D - 2);
writeln(Dom1D + 2);

writeln(Dom2D + (2, 2));
writeln(Dom3D - (1, 2, 3));

var A: [Dom2D + (1, 1)] int;

writeln(A);
for i in A.domain do
  writeln(i);
writeln(A.domain.dist == Dist2D);
