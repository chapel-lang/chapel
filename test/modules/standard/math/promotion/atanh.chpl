// atanh.chpl
// 
// AUTO-GENERATED, DO NOT EDIT.  See generate_tests/ directory.
// 
config param n = 10;
var D = {0..n};
var A : [D] real;
var B : [D] real;
var s = (1.0 - -1.0) / n;
var m = -1.0;
A = [i in D] s * i + m;
B = atanh(A);
writeln(A);
writeln(B);
