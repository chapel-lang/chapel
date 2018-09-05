use LinearAlgebra;

var A = Matrix([1.0, 2.0, 3.0],
               [3.0, 2.0, 4.0],
               [1.0, 2.0, NAN]);

var (u, s, vt) = svd(A);
