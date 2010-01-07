use driver;

const Dom1D: domain(1) distributed Dist1D = Space1;
const Dom2D: domain(2) distributed Dist2D = Space2;
const Dom3D: domain(3) distributed Dist3D = Space3;
const Dom4D: domain(4) distributed Dist4D = Space4;

var A1D: [Dom1D] int;
var A2D: [Dom2D] int;
var A3D: [Dom3D] int;
var A4D: [Dom4D] int;

writeln(Dom1D.rank);
writeln(Dom2D.rank);
writeln(Dom3D.rank);
writeln(Dom4D.rank);
writeln(A1D.rank);
writeln(A2D.rank);
writeln(A3D.rank);
writeln(A4D.rank);

