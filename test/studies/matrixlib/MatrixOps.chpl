def MMIterator(D1, D2) {
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
// Block LU with pivoting.  This version is written for a single
// locale and it assumes that the input matrix A is square and
// has identical ranges for the two dimensions of the matrix.
// The block size, blk, must be set to a valid block size - one
// that is greater than zero and less than or equal to the dimension
// of the matrix A.

  if (D.dim(1) != D.dim(2)) then
    halt("error: blockLU requires square matrix with same dimensions");

  if (blk <= 0) | (blk > D.dim(1).length) then
    halt(blk," is an invalid block size passed to blockLU");

  var A1D = D.dim(1);
  var ind: index(D);
  const zero = 0.0:A.eltType;

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

      if (A1(k,k) != zero) {
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

def IterateByBlocks(D:range,blksize) {
  var end = D.high;
  var hi: int;

  for i in D by blksize {
    hi = min(i + blksize-1,end);
    yield (i..end,i..hi,hi+1..end); 
  }   
}

