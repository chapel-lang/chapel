use Random;
use BLAS;
use C_BLAS;

config const errorThreshold = 1.e-10;

proc main() {
  test_dgemm();
}

proc test_dgemm() {
  const m = 10 : c_int,
        n = 7 : c_int,
        k = 7 : c_int;
  // Test dgemm -- do this with an array that isn't square
  var A : [{0.. #m, 0.. #k}]real,
      B : [{0.. #k, 0.. #n}]real,
      C : [{0.. #m, 0.. #n}]real,
      D : [{0.. #m, 0.. #n}]real;

  fillRandom(A);
  fillRandom(B);
  C = 0;
  const alpha = 2.0,
        beta = 0.0;

  //cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k,
  //      alpha, A[0,0], k, B[0,0], n, beta, C[0,0], n);
  gemm(A,B,C,alpha,beta);
  forall (i,j) in D.domain do D[i,j] = alpha*(+ reduce (A[i,..]*B[..,j]));
  var err = max reduce abs(C-D);
  printPass(err, "dgemm");
}


proc printPass(err, case) {
  if err > errorThreshold {
    writeln("FAILED ",case);
  } else {
    writeln("PASSED ",case);
  }
}
