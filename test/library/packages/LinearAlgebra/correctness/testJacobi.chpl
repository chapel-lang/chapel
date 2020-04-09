use LinearAlgebra;
use LinearAlgebra.Sparse;
use Math;
use Time;

config const n = 16, 
             tol = 0.000001,
             maxiter = 10000,
             threshold = 0.00001;
             

var t: Timer;
var itern: int;

// Dense

var Xdom = {1..n};
var Adom = {1..n, 1..n};

var A: [Adom] real = 0.0;

setDiag(A, 0, 2);
setDiag(A, 1, -1);
setDiag(A, -1, -1);

var X, b: [Xdom] real = 1.0;
t.start();
itern = jacobi(A,X,b, tol, maxiter);
t.stop();
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
maxerror = max reduce abs(b - dot(A,X));
writeln(maxerror < threshold);

