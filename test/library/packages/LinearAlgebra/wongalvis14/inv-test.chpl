use LinearAlgebra;

config const n=10;
config const thresh=1.0e-10;

proc inv (in A: [?Adom] ?eltType) {
  if Adom.rank != 2 then
    halt("Wrong rank for matrix inverse");

  if Adom.shape(1) != Adom.shape(2) then
    halt("Matrix inverse only supports square matrices");

  const n = Adom.shape(1);
  var ipiv : [1..3] c_int;
  
  LAPACK.getrf(lapack_memory_order.row_major, A, ipiv);
  LAPACK.getri(lapack_memory_order.row_major, A, ipiv);

  return A;
}

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
