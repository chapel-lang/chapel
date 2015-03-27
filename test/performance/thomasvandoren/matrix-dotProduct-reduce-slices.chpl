/* Chapelerific matrix dot product.
*/

use TestHelpers;

// Calculate C = A * B, then print C.
proc main() {
  timer.start();

  dotProduct(C, A, B);

  timer.stop();
  printResults();
}

proc dotProduct(ref C: [?DC] int, ref A: [?DA] int, ref B: [?DB] int)
  where DC.rank == 2 && DA.rank == 2 && DB.rank == 2
{
  // If not running with --fast (e.g. boundsChecking == true), ensure the array
  // dimensions are correct.
  if boundsChecking {
    assert(DA.dim(2) == DB.dim(1),
           "Inner dimensions of A and B do not match.");
    assert(DC.dim(1) == DA.dim(1) &&
           DC.dim(2) == DB.dim(2),
           "Outer dimentions of C, A, and B do not match.");
  }

  forall (row, col) in C.domain do
    C[row, col] = + reduce (A[row, 1..] * B[1.., col]);
}
