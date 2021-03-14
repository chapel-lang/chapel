use LinearAlgebra;
use LinearAlgebra.Sparse;
use Math;
use Time;

config const n = 128,
             tol = 0.0001,
             maxiter = 1000000,
             threshold = 0.01,
             correctness = false;


var t: Timer;
var itern: int;

// Dense

var Xdom = {0..<n};
var Adom = {0..<n, 0..<n};

var A: [Adom] real = 0.0;

setDiag(A, 0, 2);
setDiag(A, 1, -1);
setDiag(A, -1, -1);

var X, b: [Xdom] real = 1.0;
t.start();
itern = jacobi(A,X,b, tol, maxiter);
t.stop();
if !correctness {
  writeln(itern);
  writeln("LinearAlgebra.jacobi: ", t.elapsed());
}
var maxerror = max reduce abs(b - dot(A,X));
writeln(maxerror < threshold);

// Sparse

var CSDom = CSRDomain(n,n);
var CSA: [Adom] real = 0.0;
X = 1.0;

setDiag(CSA, 0, 2);
setDiag(CSA, 1, -1);
setDiag(CSA, -1, -1);

t.clear();
t.start();
itern = jacobi(CSA,X,b, tol, maxiter);
t.stop();
if !correctness {
  writeln(itern);
  writeln("LinearAlgebra.Sparse.jacobi: ", t.elapsed());
}
maxerror = max reduce abs(b - dot(A,X));
writeln(maxerror < threshold);
