use driver_domains;

proc foo(Dom, Dist) {
  writeln(Dom.expand(1));
  var A: [Dom.expand(1)] int;

  forall e in A do
    e = 2;

  writeln(A);
  writeln(dist_eq(A.domain.dist, Dist));
}

foo(Dom1D, Dist1D);
foo(Dom2D, Dist2D);
foo(Dom3D, Dist3D);
foo(Dom4D, Dist4D);

writeln(Dom2D.expand(2, 3));
writeln(Dom3D.expand(-1, 2, 3));
writeln(Dom4D.expand(2));
writeln(Dom2D32.expand(-1));
