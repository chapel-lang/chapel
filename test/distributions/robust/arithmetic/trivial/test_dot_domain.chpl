use driver_arrays;

writeln(A1D.domain);
writeln(A2D.domain);
writeln(A3D.domain);
writeln(A4D.domain);
writeln(A2D64.domain);
writeln(A1D.domain.dist == Dist1D);
writeln(A2D.domain.dist == Dist2D);
writeln(A3D.domain.dist == Dist3D);
writeln(A4D.domain.dist == Dist4D);
writeln(A2D64.domain.dist == Dist2D64);
writeln(A1D.domain.dist != [1..n1].dist);
