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

// Remove after #13515 is merged
proc setDiag (ref X: [?D] ?eltType, in k: int = 0, val: eltType = 0) 
              where isDenseArr(X){
  var start, end = 0;
  if (k >= 0) { // upper or main diagonal
    start = 1;
    end = D.shape(1) - k;
  }
  else { // lower diagonal
    start = 1 - k;
    end = D.shape(1);
  }
  forall row in {start..end} {
    X(row, row+k) = val;
  }
}

proc setDiag (ref X: [?D] ?eltType, in k: int = 0, val: eltType = 0)
                where isSparseArr(X) { 
    if D.rank != 2 then
      halt("Wrong rank for setDiag");

    if D.shape(1) != D.shape(2) then
      halt("setDiag only supports square matrices");

    var start, end = 0;
    if (k >= 0) { // upper or main diagonal
      start = 1;
      end = D.shape(1) - k;
    }
    else { // lower diagonal
      start = 1 - k;
      end = D.shape(1);
    }
    var indices : [start..end] (D.idxType, D.idxType);
    forall ind in {start..end} {
      indices[ind] = (ind, ind+k);
    }
    D.bulkAdd(indices, dataSorted=true, isUnique=true, preserveInds=false);
    forall ind in indices {
      X(ind) = val;
    }
}

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
itern = jacobi(A,X,b, tol, maxiter);
t.stop();
maxerror = max reduce abs(b - dot(A,X));
writeln(maxerror < threshold);

