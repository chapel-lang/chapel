use driver;

proc foo(param rank, type idxType, Dist, extent) {
  const D: domain(rank, idxType) dmapped Dist = rankDomain(rank, extent);
  var A: [D] real;

  for i in rankDomain(rank, extent) do
    A(i) = next();
  for i in rankDomain(rank, extent) do
    write(" ", A(i));
  writeln();
}

foo(1, int, Dist1D, -2..4);
foo(2, int, Dist2D, -2..3);
foo(3, int, Dist3D, -2..2);
foo(4, int, Dist4D, -2..1);
foo(1, int, Dist1D, 102..107);
foo(2, int, Dist2D, 102..106);
foo(3, int, Dist3D, 102..105);
foo(4, int, Dist4D, 102..104);
foo(2, int(32), Dist2D32, 2..6:int(32));
