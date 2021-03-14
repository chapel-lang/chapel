config var n : int = 8;

var D : domain(2) = {1..n, 1..n};
var A : [D] int;
var B : [D] int;

[(i,j) in D] A(i,j) = (i - 1) * n + j;

writeln(A);

[(i,j) in D] B(i,j) = A(i,j);

writeln(B);
