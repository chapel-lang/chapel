use driver;

def foo(param rank, Dist, extent) {
  const D: domain(rank) distributed Dist = rankDomain(rank, extent);
  var A: [D] real;

  for i in rankDomain(rank, extent) do
    A(i) = next();
  for i in rankDomain(rank, extent) do
    write(" ", A(i));
  writeln();
}

foo(1, Dist1D, -2..4);
foo(2, Dist2D, -2..3);
foo(3, Dist3D, -2..2);
foo(4, Dist4D, -2..1);
foo(1, Dist1D, 102..107);
foo(2, Dist2D, 102..106);
foo(3, Dist3D, 102..105);
foo(4, Dist4D, 102..104);
foo(2, Dist2D64, -2..+2);
