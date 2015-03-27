/* 2D matrix helper functions. */

use Assert;

// Transpose inputMatrix and store in outputMatrix.
proc transpose(ref outputMatrix: [?D] real, ref inputMatrix: [?E] real)
  where D.rank == 2 && E.rank == 2
{
  // If not running with --fast (e.g. boundsChecking == true), ensure the array
  // dimensions are correct.
  if boundsChecking {
    assert(D.dim(1) == E.dim(2) && D.dim(2) == E.dim(1),
           "Dimensions of outputMatrix are not transpose of inputMatrix.");
  }

  var outCols = E.dim(1),
    outRows = E.dim(2);
  forall (row, col) in D {
    outputMatrix[row, col] = inputMatrix[col, row];
  }
}

// Calculate dot product (matrix multiplication) of A and B and store result in
// C. Specifically, it does C = A * B.
proc dotProduct(ref C: [?DC] real, ref A: [?DA] real, ref B: [?DB] real)
  where DC.rank == 2 && DA.rank == 2 && DB.rank == 2
{
  // If not running with --fast (e.g. boundsChecking == true), ensure the array
  // dimensions are correct.
  if boundsChecking {
    assert(DC.dim(1) == DA.dim(1) &&
           DC.dim(2) == DB.dim(2),
           "Outer dimensions for C, A, or B do not match.");
    assert(DA.dim(2) == DB.dim(1),
           "Inner array dimensions do not match.");
  }

  // There is a more Chapelerific way to implement 2D matrix dot product, but
  // it suffers from poor performance. See tests in
  // $CHPL_HOME/test/performance/thomasvnadoren/ for more details.
  forall (row, col) in DC {
    C[row, col] = 0.0;
    for z in DA.dim(2) do
      C[row, col] += A[row, z] * B[z, col];
  }
}
