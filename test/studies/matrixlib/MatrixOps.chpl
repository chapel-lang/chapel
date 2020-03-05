// blockLU: Computes block-LU with pivoting.  The arguments are as follows:
//   A: the input array to be factored.  It must be 2D, square, and
//        its dimensions must be defined using identical ranges
//
//   blk: the blocking factor, which must be positive and less than or
//        equal to A's size.
//
//   piv: a vector used to store and output the permutations performed
//        by pivoting operations

proc blockLU(x: [?D], piv, blk) where (D.rank != 2) {
// This routine will give a compiler error if blockLU
// is called with an array that is not two dimensional.

  compilerError("blockLU factors a matrix.  The first input parameter to blockLU must be a two-dimensional array.");
}

proc blockLU(A: [?D], blk, piv: [D.dim(1)]) where (D.rank == 2) {

  // Test that the domain of A is square with the same index set for
  // each dimension.
  if (D.dim(1) != D.dim(2)) then
    halt("blockLU requires square matrix with same dimensions");

  // Test that 0 < blk <= n, where n = length of one dimension of A.
  if (blk <= 0) || (blk > D.dim(1).size) then
    halt(blk," is an invalid block size passed to blockLU");

  [i in D.dim(1)] piv(i) = i;    // initialize the pivot vector

  // Main loop of block LU uses an iterator to compute three sets of
  // index ranges -- those that are unfactored, divided into those
  // currently being factored and those that remain.

  for (UnfactoredInds, CurrentBlockInds, TrailingBlockInds) 
        in generateBlockLURanges(D, blk) {

    // Create array aliases for various submatrices of current block
    // iteration as follows:
    //
    //     |Factored    |   |Factored   |   |Factored   |
    //     |  _ _ _ _ _ |   |    _ _ _ _|   |    _ _ _ _|
    // A = | |Unfactored| = |   |A1 | A2| = |   |A11 A12|
    //     | |          |   |   |   |   |   |   |A21 A22|
    //
    // where A1 = |A11| 
    //            |A21| is the current rectangular block that
    //                  is factored in this iteration
    //
    //       A2 = |A12| 
    //            |A22| is updated after the LU factorization
    //                  of A1
  
    ref A1  = A[UnfactoredInds, CurrentBlockInds],
        A2  = A[UnfactoredInds, TrailingBlockInds],
        A11 = A[CurrentBlockInds, CurrentBlockInds],
        A21 = A[TrailingBlockInds, CurrentBlockInds],
        A12 = A[CurrentBlockInds, TrailingBlockInds],
        A22 = A[TrailingBlockInds, TrailingBlockInds];

    // First compute the LU factorization of A1...
    //
    // We loop over the rows of A11, but we eliminate the
    // the full rectangular A1 block.

    for k in CurrentBlockInds {  

      // Find pivot for kth column:
      //   identify largest magnitude element in kth column 
      //   (pivot), from the diagonal element downward 
      //   through all rows of A1
      const pivotRow = computePivotRow(A1[UnfactoredInds(k..), k..k]),
            pivot = A1[pivotRow, k];
      
      // If kth row is not row of pivot, swap rows:
      if (pivotRow != k) {

        // store row permutation in piv array
        piv(k) <=> piv(pivotRow);

        // swap values in rows k and pivotRow of the full
        // matrix A
        A[k, ..] <=> A[pivotRow, ..];
      }

      // If pivot is zero, halt.  Matrix is singular and 
      // cannot be factored.  
      if (pivot == 0) then halt("zero pivot encountered");

      // Compute the k-th elimination step: 
      //   store multipliers...
      A1[UnfactoredInds(k+1..), k] /= pivot;

      //   ..and subtract scaled kth row from remaining 
      //   unfactored rows of A1

      forall (i,j) in {UnfactoredInds(k+1..), CurrentBlockInds(k+1..)} do
        A1(i,j) -= A1(i,k) * A1(k,j);
    }

    // ... and now update A2. 
    // First update A12.

    forall j in TrailingBlockInds do
      for k in CurrentBlockInds do
        forall i in CurrentBlockInds(k+1..) do
          A12(i,j) -= A1(i,k) * A12(k,j);

    // And then update A22 -= A21*A12.
    //
    // MMIterator is used to generate the indices for this
    // matrix-matrix multiplication step.  MMIterator can
    // be optimized (e.g., for the loop nesting order and loop 
    // unrolling) separately from this blockLU routine.

    for (i,j,k1,k2) in MMIterator(A21.domain, A12.domain) do
      A22(i,j) -= A21(i,k1) * A12(k2,j);
  }
}

// The generateBlockLURanges iterator returns a 3-tuple of 
// the ranges to be used in each iteration of block LU:
//   (UnfactoredInds, CurrentBlockInds, TrailingBlockInds).
// By defining these ranges, the blockLU code is cleaner. It
// eliminates the need to test for the last iteration where a full-sized
// block may not be possible and only the factorization step (and not
// the update step) is required. 
//
// The range UnfactoredInds is equivalent to the range 
// CurrentBlockInds concatenated with the range TrailingBlockInds. 
//
// The range CurrentBlockInds has the length of blksize, except
// for the end case when the last block may not be of full size.
// This iterator takes care of this end case so that testing for
//  it is not necessary in the blockLU code itself. 
//
// The range TrailingBlockInds will be an empty range for the
// the last block iteration.  When it is empty, the loops in
// the blockLU code will not execute, so testing for this case is
// not necessary.

iter generateBlockLURanges(D:domain(2), blksize) {
  const stop = D.dim(1).high;

  for i in D.dim(1) by blksize {
    const hi = min(i + blksize-1, stop);
    yield (i..stop, i..hi, hi+1..stop); 
  }
}


// The MMIterator generates indices for a matrix multiplication loop.
// This iterator can be tuned for loop nesting order or loop unrolling,
// isolating these optimizations from the blockLU code (or any other
// code in a complete linear algebra library that uses matrix-matrix
// multiplication).

iter MMIterator(D1, D2) {
  for j in D2.dim(2) do
    for (k1, k2) in zip(D1.dim(2), D2.dim(1)) do
      for i in D1.dim(1) do
        yield (i,j,k1,k2);
}


// This function computes the pivot row for an LU pivoting operation
// by using Chapel's maxloc reduction on the absolute values in the
// matrix, dropping the maximum value on the floor using the
// underscore operator, and returning the row of the resulting index.

proc computePivotRow(A:[?D]) {
   const (_, ind) = maxloc reduce zip(abs(A), D);
   return ind(1);
}

proc blockChol(A:[?D],blk,factor:string) where (D.rank == 2) {
  if (D.dim(1) != D.dim(2)) then
    halt("error:  blockChol requires a square matrix with same dimensions");

  var A1D = D.dim(1);
  const zero = 0.0:A.eltType;
  const upper = (factor == "U");

  for (PrecedingBlockInds,CurrentBlockInds,TrailingBlockInds) in GenerateCholBlocks(A1D,blk) {

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

iter GenerateCholBlocks(D:range,blksize) {
  var start = D.low;
  var stop = D.high;
  var hi: int;

  for i in D by blksize {
    hi = min(i+blksize-1,stop);
    yield (start..i-1,i..hi,hi+1..stop);
  }
}
