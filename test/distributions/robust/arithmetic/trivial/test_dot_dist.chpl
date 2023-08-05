use driver_domains;

writeln(dist_eq(Dom1D.distribution, Dist1D));
writeln(dist_eq(Dom2D.distribution, Dist2D));
writeln(dist_eq(Dom3D.distribution, Dist3D));
writeln(dist_eq(Dom4D.distribution, Dist4D));
writeln(dist_eq(Dom2D32.distribution, Dist2D32));

// since DefaultDist is a singleton, neq would return false
if distType == DistType.default then
  writeln(true);
else
  writeln(dist_neq(Dom2D32.distribution, Dist2D));
