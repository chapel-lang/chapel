/* Matrix dot product that used + reduce on the list of products, which are
   calculated with a forall loop using the [i in A] i comprehension form.
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

  // Use promotion of * operator on slices of A and B, then reduce that result
  // with + operator.

  // This is a variation of foralls, which pushes the loop inside a +
  // reduction, then assigns the result of the reduction to C[row, col].
  forall (row, col) in DC do
    C[row, col] = + reduce ([i in DA.dim(1)] A[row, i] * B[i, col]);
}

