/* This file contains a block-LU solver function named blockLU() along
 * with its supporting functions and iterators.  It also contains a
 * main function that tests the blockLU function.
 *
 * The test matrix is read in from blockLU-Mat.dat by default, but a
 * different input file can be specified on the command line using
 * --inputfile=<filename>.  The default block size is 2, but this can
 * be overridden on the command line using --block=<blocksize>.
 *
 * This blockLU function is written for a single locale.  It uses an
 * iterator to define blocks of indices that make expressing the block
 * algorithm easier and less error-prone.  Array aliases are defined
 * with these block indices at each iteration, so that the submatrices
 * involved in the block factorization and in the subsequent updates
 * are easily referenced and modified.
 */ 


// configuration variables:
//   inputfile: a string representing the file containing matrix data
//   block:     the block size to use for the block-LU factorization

config const inputfile = "blockLU-Mat.dat";
config var block = 2;

// blockLU: Computes block-LU with pivoting.  The arguments are as follows:
//   A: the input array to be factored.  It must be 2D, square, and
//        its dimensions must be defined using identical ranges
//
//   blk: the blocking factor, which must be positive and less than or
//        equal to A's size.
//
//   piv: a vector used to store and output the permutations performed
//        by pivoting operations

def blockLU(A: [?D], blk, piv: [D.dim(1)]) where (D.rank == 2) {

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
        in generateBlockLURanges(D.dim(1), blk) {

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

    for k in CurrentBlockInds {  

      // Find pivot for kth column:
      //   identify largest magnitude element in kth column 
      //   (pivot), from the diagonal element downward 
      //   through all rows of A1
      const pivotRow = computePivotRow(A1[k.., k..k]),
            pivot = A1[pivotRow, k];
      
      // If kth row is not row of pivot, swap rows:
      if (pivotRow != k) {

        // store row permutation in piv array
        piv(k) <=> piv(pivotRow);

        // swap values in rows k and pivotRow of the full
        // matrix A
        A[k..k, ..] <=> A[pivotRow..pivotRow, ..];
      }

      // If pivot is zero, halt.  Matrix is singular and 
      // cannot be factored.  
      if (pivot == 0) then halt("zero pivot encountered");

      // Compute the k-th elimination step: 
      //   store multipliers...
      if k+1 <= A1.domain.high(1) then
        A1[k+1.., k..k] /= pivot;

      //   ..and subtract scaled kth row from remaining 
      //   unfactored rows of A1

      forall (i,j) in [UnfactoredInds(k+1..), CurrentBlockInds(k+1..)] do
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

def generateBlockLURanges(D:range, blksize) {
  const end = D.high;

  for i in D by blksize {
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


// The main function here provides a test harness for calling the
// blockLU routine

def main() {

  // Open the input file, read in the array size, ensure it's square
  var infile = file(inputfile, path='./', mode='r');
  infile.open();
  const m = infile.read(int),
        n = infile.read(int);
  if (m != n) then halt("Input matrix must be square");

  // Ensure the block size is appropriate.
  // It cannot be less than 1.  Reset to 1 if this happens.
  // It cannot exceed the size of n.  Reset to n if this happens.
  block = max(1, block);
  block = min(block, n);

  // declare a range A1D, 2D domain AD, and array (matrix) A
  const A1D = 1..n,
        AD = [A1D, A1D]; 
  var A: [AD] real;

  // Initialize A by reading values in from infile.
  for ij in AD do
    infile.read(A(ij));

  // close the input file
  infile.close();

  // declare the pivot permutation vector
  var piv: [A1D] int;  

  // print out the initial matrix
  writeln("Unfactored Matrix:\n", A, "\n");

  // perform the blocking
  blockLU(A, block, piv);

  // print out the results
  writeln("Factored Matrix:\n", A, "\n");
  writeln("Pivot Vector:\n", piv);
}
