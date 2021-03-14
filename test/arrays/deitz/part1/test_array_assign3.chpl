config var n : int = 4;

var D : domain(2) = {1..n, 1..n};
var A : [D] int;
var B : [D] int;
var C : [D] int;

[(i,j) in D] A(i,j) = (i - 1) * n + j;
[(i,j) in D] B(i,j) = (i - 1) * n + j;

writeln(A);
writeln(B);

C = A + B;

A(1,1) = 500;
B(1,1) = 500;

writeln(C);
