use driver;

const Dom1D: domain(1) distributed Dist1D = Space1;
const Dom2D: domain(2) distributed Dist2D = Space2;
const Dom3D: domain(3) distributed Dist3D = Space3;
const Dom4D: domain(4) distributed Dist4D = Space4;
const Dom2D64: domain(2, int(64)) distributed Dist2D64 = Space2D64;

record R { var x: int; var y: real; }

var A1D: [Dom1D] string;
var A2D: [Dom2D] real;
var A3D: [Dom3D] int(64);
var A4D: [Dom4D] R;
var A2D64: [Dom2D64] uint(8);

writeln(typeToString(A1D.eltType));
writeln(typeToString(A2D.eltType));
writeln(typeToString(A3D.eltType));
writeln(typeToString(A4D.eltType));
writeln(typeToString(A2D64.eltType));
