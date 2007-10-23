/* This file contains a right-looking block-LU solver function named 
 * rightBlockLU() along with its supporting functions and iterators,
 * and a solve routine that takes the LU factors to solve Ax=b.  
 *
 * This rightBlockLU function is written for a single locale.  It uses an
 * iterator to define blocks of indices that make expressing the block
 * algorithm easier and less error-prone.  Array aliases are defined
 * with these block indices at each iteration, so that the submatrices
 * involved in the block factorization and in the subsequent updates
 * are easily referenced and modified.
 */ 


// rightBlockLU: Computes block-LU with pivoting.  The arguments are as follows:
//   A: the input array to be factored.  It must be 2D, square, and
//        its dimensions must be defined using identical ranges
//
//   blk: the blocking factor, which must be positive and less than or
//        equal to A's size.
//
//   piv: a vector used to store and output the permutations performed
//        by pivoting operations

def rightBlockLU(A: [?D], blk, piv: [D.dim(1)]) where (D.rank == 2) {

  // Need to modify routine to factor rectangular system:  [A | b].
  // Also, don't pivot rows of L or return pivot vector.

  // Test that the domain of A is square with the same index set for
  // each dimension.
  if (D.dim(1) != D.dim(2)) then
    halt("blockLU requires square matrix with same dimensions");

  // Test that 0 < blk <= n, where n = length of one dimension of A.
  if (blk <= 0) || (blk > D.dim(1).length) then
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
  
    var A1  => A[UnfactoredInds, CurrentBlockInds],
        A2  => A[UnfactoredInds, TrailingBlockInds],
        A11 => A[CurrentBlockInds, CurrentBlockInds],
        A21 => A[TrailingBlockInds, CurrentBlockInds],
        A12 => A[CurrentBlockInds, TrailingBlockInds],
        A22 => A[TrailingBlockInds, TrailingBlockInds];

    // First compute the LU factorization of A1...
    //
    // We loop over the rows of A11, but we eliminate the
    // the full rectangular A1 block.

    // This is the panel factorization.  Need to add alternative
    // factorization options.  Also, need to add pipelining of
    // panel factorization.  Instead of CurrentBlockInds, have
    // CurrentPanelInds and NextPanelInds for a lookahead of one
    // (or is that two? need to check).
    for k in CurrentBlockInds {  

      // Find pivot for kth column:
      //   identify largest magnitude element in kth column 
      //   (pivot), from the diagonal element downward 
      //   through all rows of A1

      // Was not able to replace k..k with k here.  Why?
      const pivotRow = computePivotRow(A1[k.., k..k]),
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
      A1[k+1.., k] /= pivot;

      //   ..and subtract scaled kth row from remaining 
      //   unfactored rows of A1

      forall (i,j) in [UnfactoredInds(k+1..), CurrentBlockInds(k+1..)] do
        A1(i,j) -= A1(i,k) * A1(k,j);
    }

    // ... and now update A2. 
    // First update A12.

    // Add loop to update NextPanel first so that lookahead factorization
    // could occur.  Maybe use sync vars to do this?

    // Communication of A1 to other processors occurs before this
    // step.  There are options for communication that need to be reflected
    // here or in distribution.
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

def generateBlockLURanges(D:domain(2), blksize) {
  const end = D.dim(1).high;

  for i in D.dim(1) by blksize {
    const hi = min(i + blksize-1, end);
    yield (i..end, i..hi, hi+1..end); 
  }
}


// The MMIterator generates indices for a matrix multiplication loop.
// This iterator can be tuned for loop nesting order or loop unrolling,
// isolating these optimizations from the blockLU code (or any other
// code in a complete linear algebra library that uses matrix-matrix
// multiplication).

def MMIterator(D1, D2) {
  for j in D2.dim(2) do
    for (k1, k2) in (D1.dim(2), D2.dim(1)) do
      for i in D1.dim(1) do
        yield (i,j,k1,k2);
}


// This function computes the pivot row for an LU pivoting operation
// by using Chapel's maxloc reduction on the absolute values in the
// matrix, dropping the maximum value on the floor using the
// underscore operator, and returning the row of the resulting index.

def computePivotRow(A:[?D]) {
   const (_, ind) = maxloc reduce (abs(A), D);
   return ind(1);
}

def LUSolve (A: [?ADom], x: [?xDom], b: [?bDom], piv) {

   var n = piv.numElements;
   var AD1 = ADom.dim(1);

   x = b(piv);
   for j in 1..n-1 {
     for (i1, i2) in (xDom(j+1..n), AD1(j+1..n)) {
        x(i1) -= x(j)*A(i2,j);
     }
   }

   for j in (2..n) by -1 {
     x(j) /= A(j,j);
     for (i1, i2) in (xDom(1..j-1), AD1(1..j-1)) {
       x(i1) -= x(j)*A(i2, j); 
     }
   }

   x(1) /= A(1,1);

}      
