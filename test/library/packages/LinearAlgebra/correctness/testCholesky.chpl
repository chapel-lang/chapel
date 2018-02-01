use LinearAlgebra;
/*
proc cholesky(A: [] real, uplo: string = 'L') where A.rank == 2 {
  use LAPACK;
  assert(A.domain.dim(1) == A.domain.dim(2));
  assert(uplo == 'L' || uplo == 'U');
  var copy = A;
  potrf(lapack_memory_order.row_major, uplo, copy);
  if uplo == 'L' {
    // zero the upper triangle, not including the main diagonal
    for i in copy.domain.dim(1) do
      copy[i,i+1..] = 0.0;
  } else {
    // zero the lower triangle, not including the main diagonal
    for i in copy.domain.dim(1) do
      copy[i,..i-1] = 0.0;
  }
  return copy;
}

proc cholesky(A: [] complex, uplo: string = 'L') where A.rank == 2 {
  use LAPACK;
  assert(A.domain.dim(1).length == A.domain.dim(2).length);
  assert(uplo == 'L' || uplo == 'U');
  var copy = A;
  potrf(lapack_memory_order.row_major, uplo, copy);
  if uplo == 'L' {
    // zero the upper triangle, not including the main diagonal
    for i in A.domain.dim(1) do
      copy[i,i+1..] = 0.0;
  } else {
    // zero the lower triangle, not including the main diagonal
    for i in A.domain.dim(1) do
      copy[i,..i-1] = 0.0;
  }
  return copy;
}
*/
var D = {1..4, 1..4};
var A: [D] real = ((18.0, 22.0,  54.0,  42.0),
                   (22.0, 70.0,  86.0,  62.0),
                   (54.0, 86.0, 174.0, 134.0),
                   (42.0, 62.0, 134.0, 106.0));

var L = cholesky(A);
var U = cholesky(A, uplo=lowerOrUpper.upper);
writeln("A:");
writeln(A);
writeln("L:");
writeln(L);
writeln("U:");
writeln(U);
