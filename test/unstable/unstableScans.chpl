var A: [1..10] int = 1..10;

var B = + scan A;
var C = + scan [a in A] a;
var D = + scan A[1..10];
var E = maxloc scan zip(A, A.domain);

writeln(A);
writeln(B);
writeln(C);
writeln(D);
writeln(E);
