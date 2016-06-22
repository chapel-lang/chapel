use driver_domains;

record R { var x: int; var y: real; }

var A1: [Dom1D] string;
var A2: [Dom2D] real;
var A3: [Dom3D] int(64);
var A4: [Dom4D] R;
var A5: [Dom2D32] uint(8);

writeln(A1.eltType:string);
writeln(A2.eltType:string);
writeln(A3.eltType:string);
writeln(A4.eltType:string);
writeln(A5.eltType:string);
