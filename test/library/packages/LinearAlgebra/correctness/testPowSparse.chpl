use LinearAlgebra;
use LinearAlgebra.Sparse;

var D = CSRDomain(3,3);

for ii in 1..#3 do D += (ii,ii);

writeln("Real version:");
var A = CSRMatrix(D, real);
A(1,1) = 1;
A(2,2) = 2;
A(3,3) = 3;
write(matPow(A,3));

writeln("Int version:");
var B = CSRMatrix(D, int);
B(1,1) = 1;
B(2,2) = 2;
B(3,3) = 3;
write(matPow(B,3));