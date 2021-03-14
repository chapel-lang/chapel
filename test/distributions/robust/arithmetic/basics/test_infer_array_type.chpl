use driver_arrays;

A1D[5] = -1;
A2D[4,4] = -2;
A3D[3,3,3] = -3;
A4D[2,2,2,2] = -4;
A2D32[n5-2,n5+2] = -5;

outputArrays();

var x1 = A1D;
var x2 = A2D;
var x3 = A3D;
var x4 = A4D;
var x5 = A2D32;

for a in x1 do
  write(a);
writeln();

for a in x2 do
  write(a);
writeln();

for a in x3 do
  write(a);
writeln();

for a in x4 do
  write(a);
writeln();

for a in x5 do
  write(a);
writeln();
