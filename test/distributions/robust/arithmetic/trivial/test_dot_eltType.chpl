use driver_domains;

record R { var x: int; var y: real; }

var A1: [Dom1D] string;
var A2: [Dom2D] real;
var A3: [Dom3D] int(64);
var A4: [Dom4D] R;
var A5: [Dom2D32] uint(8);

writeln(typeToString(A1.eltType));
writeln(typeToString(A2.eltType));
writeln(typeToString(A3.eltType));
writeln(typeToString(A4.eltType));
writeln(typeToString(A5.eltType));
