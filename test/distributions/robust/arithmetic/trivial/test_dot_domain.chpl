use driver_arrays;

writeln(A1D.domain);
writeln(A2D.domain);
writeln(A3D.domain);
writeln(A4D.domain);
writeln(A2D32.domain);
writeln(dist_eq(A1D.domain.dist, Dist1D));
writeln(dist_eq(A2D.domain.dist, Dist2D));
writeln(dist_eq(A3D.domain.dist, Dist3D));
writeln(dist_eq(A4D.domain.dist, Dist4D));
writeln(dist_eq(A2D32.domain.dist, Dist2D32));

// since DefaultDist is a singleton, neq would return false
if distType == DistType.default then
  writeln(true);
else
  writeln(dist_neq(A1D.domain.dist, {1..n1}.dist));
