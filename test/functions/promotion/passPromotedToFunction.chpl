record R { var x; }
proc testit(ref r: R) { r.x += 1; }

var A: [1..1] R(int);
var B: [1..1] atomic int;
var I: [1..1] int = [1];

A[I].x += 1;
testit(A[I]);

writeln(A);

B[I].add(1);
writeln(B);
