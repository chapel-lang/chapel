use LinearAlgebra;

config const n=10;
config const thresh=1.0e-10;

// Modified sinMatrix function
proc sinMatrix(n) {
  var A = Matrix(n);
  const fac0 = 1.0/(n+1.0);
  const fac1 = sqrt(2.0*fac0);
  for (i,j) in {1..n,1..n} {
    A[i,j] = fac1*sin(i*j*pi*fac0) + 0.1; // modification is +0.1, otherwise the matrix is equal to its inverse
  }
  return A;
}

writeln(inv(sinMatrix(n)));
writeln();

var A = sinMatrix(n);
writeln(A);
writeln();

inv(A, true);
writeln(A);
