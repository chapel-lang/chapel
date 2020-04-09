use driver;

const D1: domain(1, int) dmapped Dist1D = {1..24};
const D2: domain(2, int) dmapped Dist2D = {1..4, 1..6};
const D3: domain(3, int) dmapped Dist3D = {1..2, 1..4, 1..3};

var A1: [D1] int = [i in D1] i;

writeln(A1);

var A2 = reshape(A1, D2);

writeln(A2);

var A3 = reshape(A2, D3);

writeln(A3);

var A4 = reshape(A3, {1..24});

writeln(A4);

writeln(dist_eq(A1.domain.dist, D1.dist));
writeln(dist_eq(A2.domain.dist, D2.dist));
writeln(dist_eq(A3.domain.dist, D3.dist));

// since DefaultDist is a singleton, neq would return false
if distType == DistType.default then
  writeln(true);
else
  writeln(dist_neq(A4.domain.dist, D1.dist));
