use LinearAlgebra;


config const n = 100;

var A = eye(n);
writeln(isEye(A));

var B = Matrix([0,0,0], [0,0,0], eltType=real);
writeln(isEye(B));
