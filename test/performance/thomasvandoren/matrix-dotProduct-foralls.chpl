/* Matrix dot product with nested forall loops and an inner serial
   accumulation loop.
*/

use TestHelpers;

// Calculate C = A * B, then print C.
proc main() {
  timer.start();

  // If not running with --fast (e.g. boundsChecking == true), ensure the array
  // dimensions are correct.
  if boundsChecking {
    assert(A.domain.dim(2) == B.domain.dim(1),
           "Inner dimensions of A and B do not match.");
    assert(C.domain.dim(1) == A.domain.dim(1) &&
           C.domain.dim(2) == B.domain.dim(2),
           "Outer dimentions of C, A, and B do not match.");
  }

  forall (row, col) in C.domain {
    // Zero out the value, in case C is reused.
    C[row, col] = 0;
    for i in A.domain.dim(2) do
      C[row, col] += A[row, i] * B[i, col];
  }

  timer.stop();
  printResults();
}

