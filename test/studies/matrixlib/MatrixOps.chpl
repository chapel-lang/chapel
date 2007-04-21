def *(A:[?D1],B:[?D2]) {
  var x = A(D1.low)*B(D2.low);
  var C:[D1.dim(1),D2.dim(2)] x.type;

  if (D1.dim(2).length != D2.dim(1).length) then
    halt("Matrix multiplication with incompatible matrices");

  for i in D1.dim(1) {
    for j in D2.dim(2) {
      for (k1,k2) in (D1.dim(2),D2.dim(1)) {
        C(i,j) += A(i,k1)*B(k2,j);
      }
    }
  }
  return C;
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
  compilerError("blockLU factors a matrix.  The first
input parameter to blockLU must be a two-dimensional array.");
}

def blockLU(A: [?D], piv: [D.dim(1)], blk) where (D.rank == 2){
  var slice0, slice1, slice2: range;
  var ind: index(D);

  if ((D.dim(1).low != D.dim(2).low) | (D.dim(1).high != D.dim(2).high)) then 
    halt("error: blockLU requires square matrix with same dimensions");

  var A1D = D.dim(1);
  for (UnfactoredInds,CurrentBlockInds,TrailingBlockInds) 
    in IterateByBlocks(A1D,blk) {
  
    var A1 => A[UnfactoredInds,CurrentBlockInds];
    var A2 => A[UnfactoredInds,TrailingBlockInds];
    var A12 => A[CurrentBlockInds,TrailingBlockInds];
    var A22 => A[TrailingBlockInds,TrailingBlockInds];

// LU factorization of A1 
    for k in CurrentBlockInds {
//    temporaries used instead of subdomains with indefinite ranges.
      slice0 = k..UnfactoredInds.high;
      slice1 = k+1..UnfactoredInds.high;
      slice2 = k+1..CurrentBlockInds.high;

      ind = amaxIndex(A(slice0,k..k));

      if (ind(1) != k) {
        piv(k) <=> piv(ind(1));
        A(k..k,A1D) <=> A(ind(1)..ind(1),A1D);
      }

      if (A1(k,k) != 0.0) {
        forall i in slice1 {
          A1(i,k) = A1(i,k)/A1(k,k);
        }
        forall (i,j) in [slice1,slice2] {
          A1(i,j) -= A1(i,k)*A1(k,j);
        }
      } 
      else halt("zero pivot encountered");
    }

// Update of A12.
    forall j in TrailingBlockInds {
      for k in CurrentBlockInds {
//    temporary range used instead of subdomain with indefinite range
        slice2 = k+1..CurrentBlockInds.high;
        forall i in slice2 {
          A12(i,j) -= A1(i,k)*A12(k,j);
        }
      }
    }
// Update of A22 -= A12*A21.
    forall (i,j) in [TrailingBlockInds, TrailingBlockInds] {
      for k in CurrentBlockInds {
        A22(i,j) -= A1(i,k)*A12(k,j);
      }
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

