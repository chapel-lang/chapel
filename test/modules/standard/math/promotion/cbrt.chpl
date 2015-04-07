// ../cbrt.chpl
// 
// AUTO-GENERATED, DO NOT EDIT.  See gen/ directory.
// 
config param n = 10;
var D = {0..n};
var A : [D] real;
var B : [D] real;
var s = (1000.0 - 0.0) / n;
var m = 0.0;
A = [i in D] s * i + m;
B = cbrt(A);
writeln(A);
writeln(B);
