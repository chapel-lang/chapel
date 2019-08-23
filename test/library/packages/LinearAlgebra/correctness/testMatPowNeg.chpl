use LinearAlgebra;

config const n=10;

proc getMatrix(n) {
  var A = Matrix(n);
  for (i,j) in {1..n,1..n} {
    A(i,j) = 1 + (i+j) * 0.01;
  }
  return A;
}

var A = getMatrix(n);
var A_n5 = matPow(A,-5);
writeln(A_n5);
