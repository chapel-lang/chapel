//Blocked Cholesky method.  Default test matrix is generated
//from the Matlab gallery - the lehmer matrix of order 10.
use IO;

config const inputfile = "lehmer10.dat";
config const upper = true;

proc main() {
  var Adat = open(inputfile, iomode.r).reader();

  const n = readSize(Adat);
  var blk = readBlk(Adat);
  var factor: string;

  // The blocksize cannot be less than 1.  Reset to 1 if this happens.
  // The blocksize cannot exceed the size of n.  Reset to n if this happens.
  blk = max(1,blk);
  blk = min(blk,n);

  var A1D = 1..n;
  const A2D = {A1D,A1D}; 
  var A: [A2D] real;
  initA(A,Adat);
  Adat.close();

  factor = if (upper) then "U" else "L";
  writeln("Unfactored Matrix:");
  writeln(A);
  writeln();

  blockChol(A,blk,factor);

  writeln("Factored Matrix:");
  writeCholFactor(A,factor);
  writeln();
}

proc blockChol(A:[?D],blk,factor:string) where (D.rank == 2) {
  if (D.dim(0) != D.dim(1)) then
    halt("error:  blockChol requires a square matrix with same dimensions");

  var A1D = D.dim(0);
  const zero = 0.0:A.eltType;
  const upper = (factor == "U");

  for (PrecedingBlockInds,CurrentBlockInds,TrailingBlockInds) in IterateByBlocks(A1D,blk) {

    ref U1 = A[PrecedingBlockInds,CurrentBlockInds];
    ref U2 = A[PrecedingBlockInds,TrailingBlockInds];
    ref A11 = A[CurrentBlockInds,CurrentBlockInds];
    ref A21 = A[CurrentBlockInds,TrailingBlockInds];
    ref L1 = A[CurrentBlockInds,PrecedingBlockInds];
    ref L2 = A[TrailingBlockInds,PrecedingBlockInds];
    ref A12 = A[TrailingBlockInds,CurrentBlockInds];

    for j in CurrentBlockInds {
      for (i,k) in {CurrentBlockInds(j..),PrecedingBlockInds} {
        if upper then  
          A11(j,i) -= U1(k,j)*U1(k,i);
        else
          A11(i,j) -= L1(j,k)*L1(i,k);
      }
    }

    for j in CurrentBlockInds {
      for k in CurrentBlockInds(..j-1) {
        if upper then
          A11(j,j) -= A11(k,j)*A11(k,j);
        else
          A11(j,j) -= A11(j,k)*A11(j,k);
      }

      if (A11(j,j) <= zero) then 
        halt("Matrix is not positive definite.");
      else
        A11(j,j) = sqrt(A11(j,j));

      for i in CurrentBlockInds(j+1..) {
        for k in CurrentBlockInds(..j-1) {
          if upper then
            A11(j,i) -= A11(k,i)*A11(k,j);
          else
            A11(i,j) -= A11(i,k)*A11(j,k);
        }
        if upper then
          A11(j,i) /= A11(j,j);
        else
          A11(i,j) /= A11(j,j);
      }
    }

    for j in CurrentBlockInds {
      for k in PrecedingBlockInds {
        for i in TrailingBlockInds {
          if upper then
            A21(j,i) -= U1(k,j)*U2(k,i);
          else
            A12(i,j) -= L1(j,k)*L2(i,k);
        }
      }
    }
    
    for k in CurrentBlockInds {
      for i in TrailingBlockInds {
        if upper then
          A21(k,i) = A21(k,i)/A11(k,k);
        else
          A12(i,k) = A12(i,k)/A11(k,k);
      }
      for (i,j) in {TrailingBlockInds,CurrentBlockInds(k+1..)} {
        if upper then
          A21(j,i) -= A11(k,j)*A21(k,i);
        else
          A12(i,j) -= A11(j,k)*A12(i,k);
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

proc writeCholFactor(A:[?D],fac:string) {
  var G:[D] A.eltType;

  if (fac == "U") {
    for i in D.dim(0) {
      for j in i..D.high(1){
        G(i,j) = A(i,j);
      }
    }
  }
  else {
    for j in D.dim(0) {
      for i in j..D.high(1) {
        G(i,j) = A(i,j);
      }
    }
  }
  writeln(G);
} 
