use driver;

const Dom1D: domain(1) distributed Dist1D = Space1;
const Dom2D: domain(2) distributed Dist2D = Space2;
const Dom3D: domain(3) distributed Dist3D = Space3;
const Dom4D: domain(4) distributed Dist4D = Space4;
const Dom2D64: domain(2, int(64)) distributed Dist2D64 = Space2D64;

var x1 = Dom1D;
var x2 = Dom2D;
var x3 = Dom3D;
var x4 = Dom4D;
var x5 = Dom2D64;

writeln(typeToString(x1.type) == typeToString(Dom1D.type));
writeln(typeToString(x2.type) == typeToString(Dom2D.type));
writeln(typeToString(x3.type) == typeToString(Dom3D.type));
writeln(typeToString(x4.type) == typeToString(Dom4D.type));
writeln(typeToString(x5.type) == typeToString(Dom2D64.type));

writeln(x1.dims() == Dom1D.dims());
writeln(x2.dims() == Dom2D.dims());
writeln(x3.dims() == Dom3D.dims());
writeln(x4.dims() == Dom4D.dims());
writeln(x5.dims() == Dom2D64.dims());
