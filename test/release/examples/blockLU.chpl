/* This file contains a block-LU solver function named blockLU() along
 * with its supporting functions and iterators.  It also contains a
 * main function that tests the blockLU function.
 *
 * The test matrix is read in from blockLU-Mat.dat by default, but a
 * different input file can be specified on the command line using
 * --inputfile=<filename>.  The default block size is 5, but this can
 * be overridden on the command line using --block=<blocksize>.
 *
 * This blockLU function is written for a single locale.  It uses an
 * iterator to define blocks of indices that make expressing the block
 * algorithm easier and less error-prone.  Array aliases are defined
 * with these block indices at each iteration, so that the submatrices
 * involved in the diagonal block factorization and subsequent updates
 * are easily referenced and modified.
 */ 


//
// configuration variables:
//   inputfile: a string representing the file containing matrix data
//   block:     the block size to use for the block-LU factorization
//
config const inputfile = "blockLU-Mat.dat";
config var block = 5;

//
// blockLU: Computes block-LU with pivoting.  The arguments are as follows:
//   A: the input array to be factored.  It must be 2D, square, and
//        its dimensions must be defined using identical ranges
//
//   blk: the blocking factor, which must be positive and less than or
//        equal to A's size.
//
//   piv: a vector used to store and output the permutations performed
//        by pivoting operations
//
def blockLU(A: [?D], blk, piv: [D.dim(1)]) where (D.rank == 2) {

  //
  // Test that the domain of A is square with the same index set for
  // each dimension.
  //
  if (D.dim(1) != D.dim(2)) then
    halt("blockLU requires square matrix with same dimensions");

  //
  // Test that 0 < blk <= n, where n = length of one dimension of A.
  //
  if (blk <= 0) || (blk > D.dim(1).length) then
    halt(blk," is an invalid block size passed to blockLU");

  [i in D.dim(1)] piv(i) = i;    // initialize the pivot vector

  //
  // Main loop of block LU uses an iterator to compute three sets of
  // index ranges -- those that are unfactored, divided into those
  // currently being factored and those that remain.
  //
  for (UnfactoredInds, CurrentBlockInds, TrailingBlockInds) 
        in generateBlockLURanges(D.dim(1), blk) {

    //
    // Create array aliases for various submatrices of current block
    // iteration
    //
    var A1  => A[UnfactoredInds, CurrentBlockInds],
        A2  => A[UnfactoredInds, TrailingBlockInds],
        A21 => A[TrailingBlockInds, CurrentBlockInds],
        A12 => A[CurrentBlockInds, TrailingBlockInds],
        A22 => A[TrailingBlockInds, TrailingBlockInds];

    //
    // LU factorization of A1
    //
    for k in CurrentBlockInds {  // loop over the current index block

      //
      // find the location of the largest remaining unfactored index
      // in the k-th column and store that value
      //
      const pivotRow = computePivotRow(A[k.., k..k]),
            pivot = A[pivotRow, k];

      //
      // perform the pivot using the swap operator and array slicing
      //
      if (pivotRow != k) {
        piv(k) <=> piv(pivotRow);
        A[k..k, ..] <=> A[pivotRow..pivotRow, ..];
      }

      //
      // abort on zero pivot
      //
      if (pivot == 0) then halt("zero pivot encountered");

      //
      // Compute the k-th elimination step: store multipliers...
      //
      A1[k+1.., k..k] /= pivot;

      //
      //   ...and subtract scaled kth row from remaining unfactored rows
      //
      forall (i,j) in [UnfactoredInds(k+1..), CurrentBlockInds(k+1..)] do
        A1(i,j) -= A1(i,k) * A1(k,j);
    }

    //
    // Update A12
    //
    forall j in TrailingBlockInds do
      for k in CurrentBlockInds do
        forall i in CurrentBlockInds(k+1..) do
          A12(i,j) -= A1(i,k) * A12(k,j);

    //
    // Update of A22 -= A21*A12.
    // MMIterator is used to generate the indices.  The loop
    // nesting order is not specified here, but in the iterator.
    // The iterator could also return tuple values for any of
    // indices, for loop unrolling, and the following code would
    // remain the same.
    //
    for (i,j,k1,k2) in MMIterator(A21.domain, A12.domain) do
      A22(i,j) -= A21(i,k1) * A12(k2,j);
  }
}


//
// The following iterator returns a 3-tuple of the ranges to be used
// in each iteration of block LU: all unfactored indices, those in the
// current block, and those following the current block.  It takes
// care of the end cases, so that testing for them is not necessary in
// the block LU code itself.
//
iterator generateBlockLURanges(D:range, blksize) {
  const end = D.high;

  for i in D by blksize {
    const hi = min(i + blksize-1, end);
    yield (i..end, i..hi, hi+1..end); 
  }
}


//
// This iterator generates indices for a matrix multiplication loop.
//
iterator MMIterator(D1, D2) {
  for j in D2.dim(2) do
    for (k1, k2) in (D1.dim(2), D2.dim(1)) do
      for i in D1.dim(1) do
        yield (i,j,k1,k2);
}


//
// This function computes the pivot row for an LU pivoting operation
// by using Chapel's maxloc reduction on the absolute values in the
// matrix, dropping the maximum value on the floor using the
// underscore operator, and returning returning the row of the
// resulting index.
//
def computePivotRow(A:[?D]) {
   const (_, ind) = maxloc reduce (abs(A), D);
   return ind(1);
}


//
// The main function here provides a test harness for calling the
// blockLU routine
//
def main() {
  //
  // Open the input file, read in the array size, ensure it's square
  //
  var infile = file(inputfile, path='./', mode='r');
  infile.open();
  const m = infile.read(int),
        n = infile.read(int);
  if (m != n) then halt("Input matrix must be square");

  //
  // Ensure the block size is appropriate.
  // It cannot be less than 1.  Reset to 1 if this happens.
  // It cannot exceed the size of n.  Reset to n if this happens.
  //
  block = max(1, block);
  block = min(block, n);

  //
  // declare a range A1D, 2D domain AD, and array (matrix) A
  //
  const A1D = 1..n,
        AD = [A1D, A1D]; 
  var A: [AD] real;

  //
  // Initialize A by reading values in from infile.
  //
  for ij in AD do
    infile.read(A(ij));

  //
  // close the input file
  //
  infile.close();

  //
  // declare the pivot permutation vector
  //
  var piv: [A1D] int;  // TODO: would like this to be index(A1D)

  //
  // print out the initial matrix
  //
  writeln("Unfactored Matrix:\n", A, "\n");

  //
  // perform the blocking
  //
  blockLU(A, block, piv);

  //
  // print out the results
  //
  writeln("Factored Matrix:\n", A, "\n");
  writeln("Pivot Vector:\n", piv);
}
