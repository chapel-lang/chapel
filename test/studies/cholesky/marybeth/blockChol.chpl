//Blocked Cholesky method.  Default test matrix is generated
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

  blockChol(A,blk);

  writeln("Factored Matrix:");
  writelower(A);
  writeln();
}

proc blockChol(A:[?D],blk) where (D.rank == 2) {
  if (D.dim(0) != D.dim(1)) then
    halt("error:  blockChol requires a square matrix with same dimensions");

  var A1D = D.dim(0);
  const zero = 0.0:A.eltType;

  for (PrecedingBlockInds,CurrentBlockInds,TrailingBlockInds) in IterateByBlocks(A1D,blk) {

    ref G1 = A[CurrentBlockInds,PrecedingBlockInds];
    ref G2 = A[TrailingBlockInds,PrecedingBlockInds];
    ref A1 = A[CurrentBlockInds,CurrentBlockInds];
    ref A2 = A[TrailingBlockInds,CurrentBlockInds];

    for j in CurrentBlockInds {
      for (i,k) in {CurrentBlockInds(j..),PrecedingBlockInds} {
          A1(i,j) -= G1(j,k)*G1(i,k);
      }
    }
    for j in CurrentBlockInds {
      for k in CurrentBlockInds(..j-1) {
        A1(j,j) -= A1(j,k)*A1(j,k);
      }

      if (A1(j,j) <= zero) then 
        halt("Matrix is not positive definite.");
      else
        A1(j,j) = sqrt(A1(j,j));

      for i in CurrentBlockInds(j+1..) {
        for k in CurrentBlockInds(..j-1) {
          A1(i,j) -= A1(i,k)*A1(j,k);
        }
        A1(i,j) /= A1(j,j);
      }
    }

    for j in CurrentBlockInds {
      for k in PrecedingBlockInds {
        for i in TrailingBlockInds {
          A2(i,j) -= G1(j,k)*G2(i,k);
        }
      }
    }
    
    for k in CurrentBlockInds {
      for i in TrailingBlockInds {
        A2(i,k) = A2(i,k)/A1(k,k);
      }
      for (i,j) in {TrailingBlockInds,CurrentBlockInds(k+1..)} {
        A2(i,j) -= A1(j,k)*A2(i,k);
      }
    }  
  }
}

iter IterateByBlocks(D:range,blksize) {
  var start = D.low;
  var stop = D.high;
  var hi: int;

  for i in D by blksize {
    hi = min(i+blksize-1,stop);
    yield (start..i-1,i..hi,hi+1..stop);
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
