use Random;
use BLAS;
use C_BLAS;

config const errorThresholdDouble = 1.e-10;
config const errorThresholdSingle = 1.e-5;

proc main() {
  test_gemm();
  test_symm();
}

proc test_gemm() {
  test_gemm_helper(real(32));
  test_gemm_helper(real(64));
  test_gemm_helper(complex(64));
  test_gemm_helper(complex(128));
}

proc test_gemm_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sgemm".format(blasPrefix(t));

  // Simple test
  {
    const m = 10 : c_int,
          n = 7 : c_int,
          k = 7 : c_int;
    // Test dgemm -- do this with an array that isn't square
    var A : [{0.. #m, 0.. #k}]t,
        B : [{0.. #k, 0.. #n}]t,
        C : [{0.. #m, 0.. #n}]t,
        D : [{0.. #m, 0.. #n}]t;

    var rng = makeRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(B);
    rng.fillRandom(C);
    D = C;
    const alpha = rng.getNext(),
          beta = rng.getNext();

    gemm(A,B,C,alpha,beta);
    forall (i,j) in D.domain do D[i,j] = beta*D[i,j]+alpha*(+ reduce (A[i,..]*B[..,j]));
    var err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Try transposing A
  {
    const m = 10 : c_int,
          n = 7 : c_int,
          k = 7 : c_int;
    // Test dgemm -- do this with an array that isn't square
    var A : [{0.. #k, 0.. #m}]t,
        B : [{0.. #k, 0.. #n}]t,
        C : [{0.. #m, 0.. #n}]t,
        D : [{0.. #m, 0.. #n}]t;

    var rng = makeRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(B);
    rng.fillRandom(C);
    D = C;
    const alpha = rng.getNext(),
          beta = rng.getNext();

    gemm(A,B,C,alpha,beta, opA=Op.T);
    forall (i,j) in D.domain do D[i,j] = beta*D[i,j]+alpha*(+ reduce (A[..,i]*B[..,j]));
    var err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Try hermitian conjugate of B
  {
    const m = 10 : c_int,
          n = 7 : c_int,
          k = 7 : c_int;
    // Test dgemm -- do this with an array that isn't square
    var A : [{0.. #m, 0.. #k}]t,
        B : [{0.. #n, 0.. #k}]t,
        C : [{0.. #m, 0.. #n}]t,
        D : [{0.. #m, 0.. #n}]t;

    var rng = makeRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(B);
    rng.fillRandom(C);
    D = C;
    const alpha = rng.getNext(),
          beta = rng.getNext();

    gemm(A,B,C,alpha,beta, opB=Op.H);
    forall (i,j) in D.domain do D[i,j] = beta*D[i,j]+alpha*(+ reduce (A[i,..]*conjg(B[j,..])));
    var err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Test leading dimension of array
  {
    const m = 10 : c_int,
          n = 7 : c_int,
          k = 7 : c_int,
          ld = 20 : c_int;
    // Test dgemm -- do this with an array that isn't square
    var A : [{0.. #m, 0.. #ld}]t,
        B : [{0.. #k, 0.. #ld}]t,
        C : [{0.. #m, 0.. #ld}]t,
        D : [{0.. #m, 0.. #ld}]t;

    var rng = makeRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(B);
    rng.fillRandom(C);
    D = C;
    const alpha = rng.getNext(),
          beta = rng.getNext();

    gemm(A[..,0.. #k],B[..,0.. #n],C[..,0.. #n],alpha,beta, ldA=ld, ldB=ld,ldC=ld);
    forall (i,j) in {0.. #m, 0.. #n} do
      D[i,j] = beta*D[i,j]+alpha*(+ reduce (A[i,0.. #k]*B[..,j]));
    var err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }


  writef("%sgemm : %i PASSED, %i FAILED \n", blasPrefix(t), passed, failed);
}

proc test_symm() {
  test_symm_helper(real(32));
  test_symm_helper(real(64));
  test_symm_helper(complex(64));
  test_symm_helper(complex(128));
}


proc test_symm_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%ssymm".format(blasPrefix(t));

  // Simple tests 1 & 2
  {
    const m = 10 : c_int,
          n = 7 : c_int;
    // Test dgemm -- do this with an array that isn't square
    var A : [{0.. #m, 0.. #m}]t,
        B : [{0.. #m, 0.. #n}]t,
        C : [{0.. #m, 0.. #n}]t,
        D : [{0.. #m, 0.. #n}]t;

    var rng = makeRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    makeSymm(A);
    rng.fillRandom(B);
    rng.fillRandom(C);
    var saveC = C;
    D = C;
    const alpha = rng.getNext(),
          beta = rng.getNext();

    symm(A, B, C, alpha, beta, uplo=CblasUpper, side=CblasLeft);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta);
    var err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    symm(A, B, C, alpha, beta, uplo=CblasLower, side=CblasLeft);
    // Do a direct multiplication as a test
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

  }

  // Simple tests 3 & 4
  {
    const m = 10 : c_int,
          n = 7 : c_int;
    // Test dgemm -- do this with an array that isn't square
    var A : [{0.. #n, 0.. #n}]t,
        B : [{0.. #m, 0.. #n}]t,
        C : [{0.. #m, 0.. #n}]t,
        D : [{0.. #m, 0.. #n}]t;

    var rng = makeRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    makeSymm(A);
    rng.fillRandom(B);
    rng.fillRandom(C);
    var saveC = C;
    D = C;
    const alpha = rng.getNext(),
          beta = rng.getNext();

    symm(A, B, C, alpha, beta, uplo=CblasUpper, side=CblasRight);
    // Do a direct multiplication as a test
    gemm(B,A,D,alpha,beta);
    var err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    symm(A, B, C, alpha, beta, uplo=CblasLower, side=CblasRight);
    // Do a direct multiplication as a test
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

  }
  writef("%s : %i PASSED, %i FAILED \n", name, passed, failed);
}

proc trackErrors(name, err, errorThreshold, ref passed, ref failed, ref tests) {
  if err > errorThreshold {
    failed += 1;
    writef("%s : Failure on test %i : %r\n",name, tests, err);
  } else {
    passed += 1;
  }
  tests+=1;
}

proc blasError(type t) {
  select t {
    when real(32) do return errorThresholdSingle;
    when real(64) do return errorThresholdDouble;
    when complex(64) do return errorThresholdSingle;
    when complex(128) do return errorThresholdDouble;
  }
}

proc blasPrefix(type t) {
  select t {
    when real(32) do return "s";
    when real(64) do return "d";
    when complex(64) do return "c";
    when complex(128) do return "z";
  }
}

// Make a symmetric matrix
// This does this explicitly, by making the lower triangular portion
// equal to the upper triangular
proc makeSymm(A : [?Adom]) {
  for (i,j) in Adom {
    if i < j then A[i,j] = A[j,i];
  }
}


// No-ops when conjugating real numbers...
inline proc conjg(x : real(32)) {
  return x;
}

inline proc conjg(x : real(64)) {
  return x;
}
