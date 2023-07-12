use LinearAlgebra;


config const n = 100;
config const m = 200;

var A = Matrix(n,m,eltType=int(64));

writeln(isZero(A));

var B = eye(n);
writeln(isZero(B));

