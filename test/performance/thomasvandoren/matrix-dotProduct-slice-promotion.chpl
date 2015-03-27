/* Matrix dot product using slices and promotion of * operator on slices.
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

  forall (row, col) in C.domain {
    // Zero out the value, in case C is reused.
    C[row, col] = 0;

    // Use promotion of * operator on slices of A and B. Then serially iterate
    // through result, adding each value to C[row, col].
    //
    // This gets a whole lot slower if a temp is used for the array result,
    // e.g. const values = A[row, 1..] * B[1.., col];
    for value in A[row, 1..] * B[1.., col] do
      C[row, col] += value;
  }
}
