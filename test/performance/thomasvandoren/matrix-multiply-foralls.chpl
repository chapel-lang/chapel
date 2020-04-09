/* Matrix dot product with nested forall loops and an inner serial
   accumulation loop.
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
  checkDims(DC, DA, DB);

  forall (row, col) in DC {
    // Zero out the value, in case C is reused.
    C[row, col] = 0;
    for i in DA.dim(1) do
      C[row, col] += A[row, i] * B[i, col];
  }
}

