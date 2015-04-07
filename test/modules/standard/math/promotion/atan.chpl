// ../atan.chpl
// 
// AUTO-GENERATED, DO NOT EDIT.  See gen/ directory.
// 
config param n = 10;
var D = {0..n};
var A : [D] real;
var B : [D] real;
var s = (10.0 - -10.0) / n;
var m = -10.0;
A = [i in D] s * i + m;
B = atan(A);
writeln(A);
writeln(B);
