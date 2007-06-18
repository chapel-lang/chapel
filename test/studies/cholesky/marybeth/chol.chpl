//Unblocked Cholesky method.  Default test matrix is generated
//from the Matlab gallery - the lehmer matrix of order 10.

config const inputfile = "lehmer10.dat";

def main() {
  var Adat = file(inputfile,path='./',mode='r');
  Adat.open();

  const n = readSize(Adat);
  var blk = readBlk(Adat);

  // The blocksize cannot be less than 1.  Reset to 1 if this happens.
  // The blocksize cannot exceed the size of n.  Reset to n if this happens.
  blk = max(1,blk);
  blk = min(blk,n);

  var A1D = 1..n;
  const A2D = [A1D,A1D]; 
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

def chol(A:[?D]) where (D.rank == 2) {
  if (D.dim(1) != D.dim(2)) then
    halt("error:  chol requires a square matrix with same dimensions");

  var D1 = D.dim(1);
  const zero = 0.0:A.eltType;

  for j in [D1] {
    for k in [D1(..j-1)] {
      A(j,j) -= A(j,k)*A(j,k);
    }

    if (A(j,j) <= zero) then 
      halt("Matrix is not positive definite.");
    else
      A(j,j) = sqrt(A(j,j));

    for i in [D1(j+1..)] {
      for k in [D1(..j-1)] {
        A(i,j) -= A(i,k)*A(j,k);
      }
      A(i,j) /= A(j,j);
    }
  }
}

def readSize(Adat) {
  var n: int;

  Adat.read(n);
  return n;
} 

def readBlk(Adat) {
  var blk: int;

  Adat.read(blk);
  return blk;
} 

def initA(A,Adat){

  for ij in A.domain {
    Adat.read(A(ij));
  }
}

def writelower(A:[?D]) {
  var L:[D] A.eltType;

  for i in D.dim(1) {
    for j in D.low(1)..i {
      L(i,j) = A(i,j);
    }
  }
  writeln(L);
}

