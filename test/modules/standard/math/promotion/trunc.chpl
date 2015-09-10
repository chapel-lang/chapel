// trunc.chpl
// 
// AUTO-GENERATED, DO NOT EDIT.  See generate_tests/ directory.
// 
config param n = 10;
var D = {0..n};
var A : [D] real;
var B : [D] real;
var s = (7.0 - -7.0) / n;
var m = -7.0;
A = [i in D] s * i + m;
B = trunc(A);
writeln(A);
writeln(B);
