use LinearAlgebra;

config const n=10;
config const thresh=1.0e-10;

proc sinMatrix(n) {
  var A = Matrix(n);
  const fac0 = 1.0/(n+1.0);
  const fac1 = sqrt(2.0*fac0);
  for (i,j) in {1..n,1..n} {
    A[i,j] = fac1*sin(i*j*pi*fac0);
  }
  return A;
}

writeln(inv(sinMatrix(n)));
var A = sinMatrix(n);
inv(A, true);
writeln(A);
