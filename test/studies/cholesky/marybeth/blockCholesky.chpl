// Blocked Cholesky method.  Default test matrix is generated
// from the Matlab gallery - the lehmer matrix of order 10.
//
// This algorithm can compute the lower or the upper triangular
// factorization.
// 
// This version of the blocked Cholesky method uses a new
// iterator for the blocked outer loop.  This iterator
// returns subdomains.  That way, when the array slices are
// created, conditional array aliases are not needed to handle
// either the lower or the upper triangular factorization cases.
// The subdomains are defined by the iterator according to the 
// value of the variable "upper."  
//
// These subdomains and the variable "upper" are passed to the
// iterators for the blas and unblocked Cholesky loop nests.  
// These iterators compute the indices according to the value
// of upper, iterating through the domains accordingly.
//
// There is one place in the unblocked factorization loop where
// the value of upper is checked.  More work is needed to eliminate
// this check.
//
// This routine may not run that optimally since the iterators are
// more complicated and may not be able to be inlined.  However, it
// is an interesting exercise to write some blas loops this way and
// to write a general Cholesky factorization routine that handles
// both the upper and lower choice in one routine.
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

  for (DG1, DG2, DA1, DA2) in Block(A1D,blk,upper) {

    ref G1 = A[DG1];
    ref G2 = A[DG2];
    ref A1 = A[DA1];
    ref A2 = A[DA2];
 
    for (ij, jk, ik) in IterSyrk(DG1, upper) {
      A(ij) -= G1(jk)*G1(ik);
    }

    for (diag, iRange, jRange, nextRange) in IterChol(DA1,upper) {

      A1(diag) -= dotProd(A1(iRange,jRange),A1(iRange,jRange));

      if (A1(diag) <= zero) then 
        halt("Matrix is not positive definite.");
      else
        A1(diag) = sqrt(A1(diag));
  
      if upper then
        for i in nextRange {
          A1(jRange,i..i) -= dotProd(A1(iRange,i..i),A1(iRange,jRange));
          A1(jRange,i..i) /= A1(diag);
        }
      else 
        for i in nextRange { 
          A1(i..i,iRange) -= dotProd(A1(i..i,jRange),A1(iRange,jRange));
          A1(i..i,iRange) /= A1(diag);  
        }
    }

    for (ij, jk, ik) in IterGemm(DG1, DG2, upper) {
      A2(ij) -= G1(jk)*G2(ik);
    }

    for (diag, ij, iRange1, iRange2, jRange1, jRange2) in IterTrsm(DA2, upper) {
      A2(ij) -= dotProd(A1(iRange1,jRange1),A2(iRange2,jRange2));
      A2(ij) /= A1(diag);
    }
  }
}

iter Block(D:range,blksize,upper) {
// This iterator defines the subdomains used in each iteration
// of blocked Cholesky.  
  var start = D.low;
  var stop = D.high;
  var hi: int;

  for i in D by blksize {
    hi = min(i+blksize-1,stop);
    if upper {
      var D1 = {start..i-1,i..hi};
      var D2 = {start..i-1,hi+1..stop};     
      var D3 = {i..hi,i..hi};
      var D4 = {i..hi,hi+1..stop};
      yield (D1, D2, D3, D4);
    }
    else {
      var D1 = {i..hi,start..i-1};
      var D2 = {hi+1..stop,start..i-1};     
      var D3 = {i..hi,i..hi};
      var D4 = {hi+1..stop,i..hi};
      yield (D1, D2, D3, D4);
    }
  
  }
}

iter IterSyrk(D, upper) {
//  This iterator computes the indices for
//  the computation C += A*A' or C += A'*A,
//  where only the upper or lower half of C
//  is stored.
//  For this version, we are considering
//  only two cases, based on whether or not
//  upper is true.  If upper is true, we are
//  computing C = A'*A.  If upper is false,
//  we are computing C = A*A'.  There are two
//  other cases provided in ssyrk, but they
//  aren't available here.

  const rows = D.dim(0);
  const cols = D.dim(1);

  if (upper) then
    for j in cols do
      for (i,k) in {cols(..j), rows} do
        yield ((i,j), (k,i), (k,j));
  else
    for j in rows do
      for (k,i) in {cols, rows(j..)} do
        yield((i,j), (j,k), (i,k));
}  

iter IterChol(D,upper) {
// This iterator computes the indices and ranges for
// the non-blocked cholesky factorization of the current 
// diagonal block of A.
  const rows = D.dim(0);
  for j in rows do
    if upper then
      yield((j,j), rows(..(j-1)), j..j, rows((j+1)..));
    else
      yield((j,j), j..j, rows(..(j-1)), rows((j+1)..));
} 

iter IterGemm(D1, D2, upper) {
//  This iterator computes the matrix-matrix multiplication
//  of A2 = G1*G2, with the correct transposes for either
//  the lower or upper triangular case.
  if upper then
    for i in D1.dim(1) do
      for k in D1.dim(0) do
        for j in D2.dim(1) do
          yield ((i,j), (k,i), (k,j));
  else
    for j in D1.dim(0) do
      for k in D1.dim(1) do
        for i in D2.dim(0) do
          yield ((i,j), (j,k), (i,k));
}  

iter IterTrsm(D, upper) {
// This iterator computes the indices and ranges used in the
// multiple right-hand side solve.  There are more cases for
// trsm to consider.  The two that are included here are the
// only two needed by the upper triangular and lower triangular
// Cholesky factorizations.
  const Drows = D.dim(0);
  const Dcols = D.dim(1);

  if upper then 
    for j in Dcols do
      for i in Drows do
        yield((i,i), (i,j), Drows(..i-1), Drows(..i-1), i..i, j..j);
  else
    for i in Drows do
      for j in Dcols do
        yield ((j,j), (i,j), j..j, i..i, Dcols(..j-1), Dcols(..j-1));
}

proc dotProd(A, B) {
// This routine computes the dot product of A and B.
  return (+ reduce(A*B));
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
      for j in i..D.high(1) {
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
