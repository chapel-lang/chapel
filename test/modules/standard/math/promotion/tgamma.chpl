// tgamma.chpl
// 
// AUTO-GENERATED, DO NOT EDIT.  See generate_tests/ directory.
// 
config param n = 10;
var D = {0..n};
var A : [D] real;
var B : [D] real;
var s = (7.0 - -6.5) / n;
var m = -6.5;
A = [i in D] s * i + m;
B = tgamma(A);
writeln(A);
writeln(B);
