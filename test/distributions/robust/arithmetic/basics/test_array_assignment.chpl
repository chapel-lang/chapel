use driver_arrays;

var i = 0;
proc next() { i += 1; return i; }

for e in A1D do e = next();
for e in A2D do e = next();
for e in A3D do e = next();
for e in A4D do e = next();
for e in A2D32 do e = next();

outputArrays();

var x1: [Dom1D] int;
var x2: [Dom2D] int;
var x3: [Dom3D] int;
var x4: [Dom4D] int;
var x5: [Dom2D32] int;

x1 = A1D;
x2 = A2D;
x3 = A3D;
x4 = A4D;
x5 = A2D32;

writeln(x1);
writeln(x2);
writeln(x3);
writeln(x4);
writeln(x5);
