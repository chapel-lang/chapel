// ../log1p.chpl
// 
// AUTO-GENERATED, DO NOT EDIT.  See gen/ directory.
// 
config param n = 10;
var D = {0..n};
var A : [D] real;
var B : [D] real;
var s = (10.0 - 1.0) / n;
var m = 1.0;
A = [i in D] s * i + m;
B = log1p(A);
writeln(A);
writeln(B);
