use LinearAlgebra;

proc main() {
  var A = Matrix(10);
  A = 2;

  // Depends on BLAS
  var C = dot(A,A);
}
