def *(A:[?D1],B:[?D2]) {
  var x = A(D1.low)*B(D2.low);
  var C:[D1.dim(1),D2.dim(2)] x.type;

  if (D1.dim(2).length != D2.dim(1).length) then
    halt("Matrix multiplication with incompatible matrices");

  for (i,j,k1,k2) in MMIterator(D1, D2) {
    C(i,j) += A(i,k1)*B(k2,j);
  }
  return C;
}

iterator MMIterator(D1, D2) {
  for j in D2.dim(2) do 
    for (k1,k2) in (D1.dim(2),D2.dim(1)) do 
      for i in D1.dim(1) do 
        yield (i,j,k1,k2);
}

def maxIndex(A:[?D]) {
   var (_,ind) = maxloc reduce (A,D);
   return ind;
}

def amaxIndex(A:[?D]) {
   var (_,ind) = maxloc reduce (abs(A),D);
   return ind;
}

def blockLU(x: [?D], piv, blk) where (D.rank != 2) {
// This routine will give a compiler error if blockLU
// is called with an array that is not two dimensional.

  compilerError("blockLU factors a matrix.  The first
input parameter to blockLU must be a two-dimensional array.");
}

def blockLU(A: [?D], piv: [D.dim(1)], blk) where (D.rank == 2){
// Block LU with pivoting.
// In this version, A1D is a range, not a 1D domain.  The iterator,
// IterateByBlocks returns ranges, not subdomains of A1D.  Temporary
// ranges, slice1 and slice2 are used to define what should be
// subdomains sliced by indefinite ranges.

  if (D.dim(1) != D.dim(2)) then
    halt("error: blockLU requires square matrix with same dimensions");

  var A1D = D.dim(1);
  var ind: index(D);

  for (UnfactoredInds,CurrentBlockInds,TrailingBlockInds) 
    in IterateByBlocks(A1D,blk) {
  
    var A1 => A[UnfactoredInds,CurrentBlockInds];
    var A2 => A[UnfactoredInds,TrailingBlockInds];
    var A21 => A[TrailingBlockInds,CurrentBlockInds];
    var A12 => A[CurrentBlockInds,TrailingBlockInds];
    var A22 => A[TrailingBlockInds,TrailingBlockInds];

// LU factorization of A1 
    for k in CurrentBlockInds {

      ind = amaxIndex(A(UnfactoredInds(k..),k..k));

      if (ind(1) != k) {
        piv(k) <=> piv(ind(1));
        A(k..k,..) <=> A(ind(1)..ind(1),..);
      }

      if (A1(k,k) != 0.0) {
        forall i in UnfactoredInds(k+1..) {
          A1(i,k) = A1(i,k)/A1(k,k);
        }
        forall (i,j) in [UnfactoredInds(k+1..),CurrentBlockInds(k+1..)] {
          A1(i,j) -= A1(i,k)*A1(k,j);
        }
      } 
      else halt("zero pivot encountered");
    }

// Update of A12.
    forall j in TrailingBlockInds {
      for k in CurrentBlockInds {
        forall i in CurrentBlockInds(k+1..) {
          A12(i,j) -= A1(i,k)*A12(k,j);
        }
      }
    }
// Update of A22 -= A21*A12.
    for (i,j,k1,k2) in MMIterator(A21.domain,A12.domain) {
      A22(i,j) -= A21(i,k1)*A12(k2,j);
    }
  }
}

iterator IterateByBlocks(D:range,blksize) {
  var n = D.length;
  var lo,hi: int;

  for i in D by blksize {
    lo = i;
    hi = min(i + blksize-1,n);
    yield (lo..n,i..hi,hi+1..n); 
  }   
}

