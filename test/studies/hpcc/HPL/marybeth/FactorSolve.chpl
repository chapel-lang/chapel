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
//   A: the input array to be factored.  It must be 2D and of size 
//      n by n+1.  A = [Asquare | b] on entry and [L U y] on exit.
//
//   blk: the blocking factor, which must be positive and less than or
//        equal to A's size.
//**** Need to modify routine to delay row swaps until after panel 
//     factorization

proc rightBlockLU(A: [?D], blk) where (D.rank == 2) {

  // Test that 0 < blk <= n, where n = length of one dimension of A.
  if (blk <= 0) || (blk > D.dim(1).size) then
    halt(blk," is an invalid block size passed to blockLU");

  var piv: [D.dim(1)] int;
  [i in D.dim(1)] piv(i) = i;    // initialize the pivot vector

  // Main loop of block LU uses an iterator to compute four sets of
  // index ranges -- those that are unfactored, divided into those
  // currently being factored and those that remain.  The row and
  // column indices of the remaining submatrix are specified separately
  // to accomodate the rectangular input matrix A.

  for (UnfactoredInds, CurrentPanelInds, TrailingRows, TrailingCols) 
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
  
    ref A1  = A[UnfactoredInds, CurrentPanelInds],
        A2  = A[UnfactoredInds, TrailingCols],
        A11 = A[CurrentPanelInds, CurrentPanelInds],
        A21 = A[TrailingRows, CurrentPanelInds],
        A12 = A[CurrentPanelInds, TrailingCols],
        A22 = A[TrailingRows, TrailingCols];

    // First compute the LU factorization of A1...
    //
    // We loop over the rows of A11, but we eliminate the
    // the full rectangular A1 block.

    // This is the panel factorization.  Need to add alternative
    // factorization options.  Also, need to add pipelining of
    // panel factorization.  Instead of CurrentPanelInds, have
    // CurrentPanelInds and NextPanelInds for a lookahead of one
    // (or is that two? need to check).
    for k in CurrentPanelInds {  

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
        //** swap only in panel and save remainder of swaps for
        // for update later
        A[k, ..] <=> A[pivotRow, ..];
      }

      // If pivot is zero, halt.  Matrix is singular and 
      // cannot be factored.  
      if (pivot == 0) then halt("zero pivot encountered");

      // Compute the k-th elimination step: 
      //   store multipliers...
      if k+1 <= A1.domain.high(1) then
        A1[k+1.., k] /= pivot;

      //   ..and subtract scaled kth row from remaining 
      //   unfactored rows of A1

      forall (i,j) in {UnfactoredInds(k+1..), CurrentPanelInds(k+1..)} do
        A1(i,j) -= A1(i,k) * A1(k,j);
    }

    // ... and now update A2. 
    // First update A12.

    // Add loop to update NextPanel first so that lookahead factorization
    // could occur.  Maybe use sync vars to do this?

    // Delay of row swaps needs to be incorporated into the next two
    // loop nests.  Rows of A need to be swapped with rows of U.  

    // Communication of A1 to other processors occurs before this
    // step.  There are options for communication that need to be reflected
    // here or in distribution.
    // A1 (nb x nb) is broadcast across processors in process row.
    ref U = A11; // Want each processor in process row to have local copy of A1.
    for columnblk in blkIter(TrailingCols,blk) {   // TODO: would like forall
      ref Ublk = A12[CurrentPanelInds,columnblk];
      //On processor that owns Ublk, do the following update:
      forall j in columnblk do
        for k in CurrentPanelInds do
          forall i in CurrentPanelInds(k+1..) do
            Ublk(i,j) -= U(i,k) * Ublk(k,j);
    }

    // And then update A22 -= A21*A12.
    //
    // All process columns and rows are involved in this step.
    // Blocks of L (nb x nb blocks of A21) are broadcast across 
    //   processors in process row.
    // Blocks of U (nb x nb blocks of A21) are broadcast across 
    //   processors in process column.
    // Computation occurs on processor that contains Ablk (nb x nb).
    for (rowblk, columnblk) in blkIter2D(TrailingRows,TrailingCols,blk) {
      ref Lblk = A21[rowblk,CurrentPanelInds];
      ref Ublk = A12[CurrentPanelInds,columnblk];
      ref Ablk = A22[rowblk,columnblk];
      // On processor that owns Ablk, do the following computation:
      for (i,j) in {rowblk, columnblk} do
        for k in CurrentPanelInds do
          Ablk(i,j) -= Lblk(i,k) * Ublk(k,j);
      }
    }
  
}

// The generateBlockLURanges iterator returns a 4-tuple of 
// the ranges to be used in each iteration of block LU:
//   (UnfactoredInds, CurrentPanelInds, TrailingRows, TrailingCols).
// By defining these ranges, the blockLU code is cleaner. It
// eliminates the need to test for the last iteration where a full-sized
// block may not be possible and only the factorization step (and not
// the update step) is required. 
//
// The range UnfactoredInds is equivalent to the range 
// CurrentPanelInds concatenated with the range TrailingRows. 
//
// The range CurrentPanelInds has the length of blksize, except
// for the end case when the last block may not be of full size.
// This iterator takes care of this end case so that testing for
//  it is not necessary in the blockLU code itself. 
//
// The range TrailingRows will be an empty range for the
// the last block iteration.  When it is empty, the loops in
// the blockLU code will not execute, so testing for this case is
// not necessary.

iter generateBlockLURanges(D:domain(2), blksize) {
  const stop = D.dim(1).high;
  const endcol = D.dim(2).high;

  for i in D.dim(1) by blksize {
    const hi = min(i + blksize-1, stop);
    yield (i..stop, i..hi, hi+1..stop, hi+1..endcol); 
  }
}

iter blkIter(indRange, blksize) {
  const stop = indRange.high;
 
  for i in indRange by blksize {
    const hi = min(i+blksize-1, stop);
    yield i..hi;
  }
}

iter blkIter2D(rowRange, colRange, blksize) {
  const rowEnd = rowRange.high;
  const colEnd = colRange.high;
 
  for i in rowRange by blksize {
    const rowhi = min(i+blksize-1, rowEnd);
    for j in colRange by blksize {
      const colhi = min(j+blksize-1, colEnd);
      yield (i..rowhi, j..colhi);
    }
  }
}

// This function computes the pivot row for an LU pivoting operation
// by using Chapel's maxloc reduction on the absolute values in the
// matrix, dropping the maximum value on the floor using the
// underscore operator, and returning the row of the resulting index.

proc computePivotRow(A:[?D]) {
   const (_, ind) = maxloc reduce zip(abs(A), D);
   return ind(1);
}

//  The LU solve routine takes A = [L U y] and solves for x.
proc LUSolve (A: [?ADom], x: [?xDom]) {

   var n = ADom.dim(1).size;
   var AD1 = ADom.dim(1);
   ref b = A(.., n+1);

   x = b;
   for j in (2..n) by -1 {
     x(j) /= A(j,j);
     for (i1, i2) in zip(xDom(1..j-1), AD1(1..j-1)) {
       x(i1) -= x(j)*A(i2, j); 
     }
   }

   x(1) /= A(1,1);

}      
