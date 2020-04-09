use driver_domains;

writeln(dist_eq(Dom1D.dist, Dist1D));
writeln(dist_eq(Dom2D.dist, Dist2D));
writeln(dist_eq(Dom3D.dist, Dist3D));
writeln(dist_eq(Dom4D.dist, Dist4D));
writeln(dist_eq(Dom2D32.dist, Dist2D32));

// since DefaultDist is a singleton, neq would return false
if distType == DistType.default then
  writeln(true);
else
  writeln(dist_neq(Dom2D32.dist, Dist2D));
