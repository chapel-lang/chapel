//Unblocked Cholesky method.  Default test matrix is generated
//from the Matlab gallery - the lehmer matrix of order 10.
use IO;

config const inputfile = "lehmer10.dat";

proc main() {
  var Adat = open(inputfile, iomode.r).reader();

  const n = readSize(Adat);
  var blk = readBlk(Adat);

  // The blocksize cannot be less than 1.  Reset to 1 if this happens.
  // The blocksize cannot exceed the size of n.  Reset to n if this happens.
  blk = max(1,blk);
  blk = min(blk,n);

  var A1D = 1..n;
  const A2D = {A1D,A1D}; 
  var A: [A2D] real;
  initA(A,Adat);
  Adat.close();

  writeln("Unfactored Matrix:");
  writeln(A);
  writeln();

  chol(A);

  writeln("Factored Matrix:");
  writelower(A);
  writeln();
}

proc chol(A:[?D]) where (D.rank == 2) {
  if (D.dim(0) != D.dim(1)) then
    halt("error:  chol requires a square matrix with same dimensions");

  var D1 = D.dim(0);
  const zero = 0.0:A.eltType;

  for j in D1 {
    for k in D1(..j-1) {
      A(j,j) -= A(j,k)*A(j,k);
    }

    if (A(j,j) <= zero) then 
      halt("Matrix is not positive definite.");
    else
      A(j,j) = sqrt(A(j,j));

    for i in D1(j+1..) {
      for k in D1(..j-1) {
        A(i,j) -= A(i,k)*A(j,k);
      }
      A(i,j) /= A(j,j);
    }
  }
}

proc readSize(Adat) {
  var n: int;

  Adat.read(n);
  return n;
} 

proc readBlk(Adat) {
  var blk: int;

  Adat.read(blk);
  return blk;
} 

proc initA(A,Adat){

  for ij in A.domain {
    Adat.read(A(ij));
  }
}

proc writelower(A:[?D]) {
  var L:[D] A.eltType;

  for i in D.dim(0) {
    for j in D.low(1)..i {
      L(i,j) = A(i,j);
    }
  }
  writeln(L);
}

