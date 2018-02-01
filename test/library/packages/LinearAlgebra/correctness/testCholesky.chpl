use LinearAlgebra;

var D = {1..4, 1..4};
var A: [D] real = ((18.0, 22.0,  54.0,  42.0),
                   (22.0, 70.0,  86.0,  62.0),
                   (54.0, 86.0, 174.0, 134.0),
                   (42.0, 62.0, 134.0, 106.0));

var L = cholesky(A);
var U = cholesky(A, lower=false);
writeln("A:");
writeln(A);
writeln("L:");
writeln(L);
writeln("U:");
writeln(U);
