/* 2D matrix helper functions.
 *
 */

// Returns transposed version of input matrix. User is responsible for making
// sure matrix is two dimensions.
//
// TODO: This is not ideal due to the local temp array and domain, which is
//       allocated every time this is called. (thomasvandoren, 2014-07-07)
//
// TODO: Verify inputMatrix is two dimensions. (thomasvandoren, 2014-07-07)
proc transpose(ref inputMatrix: [] real) {
  var resultCols = inputMatrix.domain.dim(1),
    resultRows = inputMatrix.domain.dim(2),
    result: [{resultRows, resultCols}] real;
  forall (row, col) in result.domain {
    result[row, col] = inputMatrix[col, row];
  }
  return result;
}

// Returns dot product (matrix multiplication) of A and B. Specifically, it
// does A * B. User is responsible for making sure A and B are the correct size
// and shape.
//
// TODO: This is not ideal due to the local temp array and domain, which are
//       allocated every time this is called. (thomasvandoren, 2014-07-07)
//
// TODO: Verify A and B are the correct size and shape.
//       (thomasvandoren, 2014-07-07)
proc dotProduct(ref A: [] real, ref B: [] real) {
  var rows = A.domain.dim(1),
    cols = B.domain.dim(2),
    result: [{rows, cols}] real;
  forall (row, col) in result.domain {
    result[row, col] = + reduce (A[row, 1..] * B[1.., col]);
  }
  return result;
}

// This is the same as dotProduct above, except it is an operator override so a
// user would actually write A * B.
//
// TODO: This doesn't work because I can't figure out how to tell the compiler
//       to use the normal element wise product of two 1D arrays for the *
//       inside the function. (thomasvandoren, 2014-07-07)
/* proc *(ref A: [domain(2)] real, ref B: [domain(2)] real) { */
/*   var cols = A.domain.dim(2), */
/*     rows = B.domain.dim(1), */
/*     result: [{cols, rows}] real; */
/*   forall (col, row) in result.domain { */
/*     result[col, row] = + reduce (A[1.., row] * B[col, 1..]); */
/*   } */
/*   return result; */
/* } */
