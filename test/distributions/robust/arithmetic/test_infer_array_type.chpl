use driver;

const Dom1D: domain(1) distributed Dist1D = Space1;
const Dom2D: domain(2) distributed Dist2D = Space2;
const Dom3D: domain(3) distributed Dist3D = Space3;
const Dom4D: domain(4) distributed Dist4D = Space4;
const Dom2D64: domain(2, int(64)) distributed Dist2D64 = Space2D64;

var A1D: [Dom1D] int;
var A2D: [Dom2D] int;
var A3D: [Dom3D] int;
var A4D: [Dom4D] int;
var A2D64: [Dom2D64] int;

A1D[5] = -1;
A2D[4,4] = -2;
A3D[3,3,3] = -3;
A4D[2,2,2,2] = -4;
A2D64[n5-2,n5-2] = -5;

var x1 = A1D;
var x2 = A2D;
var x3 = A3D;
var x4 = A4D;
var x5 = A2D64;

for a in A1D do
  write(a);
writeln();

for a in A2D do
  write(a);
writeln();

for a in A3D do
  write(a);
writeln();

for a in A4D do
  write(a);
writeln();

for a in A2D64 do
  write(a);
writeln();
