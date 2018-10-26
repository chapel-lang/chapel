record R { var x; }
proc testit(ref r: R) { r.x += 1; }

var A: [1..1] R(int);
var I: [1..1] int = [1];

A[I].x += 1;
testit(A[I]);

writeln(A);

