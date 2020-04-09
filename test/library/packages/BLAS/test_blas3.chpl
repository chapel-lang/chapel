use Random;
use BLAS;
use blas_helpers;
use IO;

proc main() {
  test_gemm();
  test_symm();
  test_hemm();
  test_syrk();
  test_herk();
  test_syr2k();
  test_her2k();
  test_trmm();
  test_trsm();
}

proc test_gemm() {
  test_gemm_helper(real(32));
  test_gemm_helper(real(64));
  test_gemm_helper(complex(64));
  test_gemm_helper(complex(128));
}

proc test_symm() {
  test_symm_helper(real(32));
  test_symm_helper(real(64));
  test_symm_helper(complex(64));
  test_symm_helper(complex(128));
}

proc test_hemm() {
  test_hemm_helper(complex(64));
  test_hemm_helper(complex(128));
}

proc test_syrk() {
  test_syrk_helper(real(32));
  test_syrk_helper(real(64));
  test_syrk_helper(complex(64));
  test_syrk_helper(complex(128));
}

proc test_herk() {
  test_herk_helper(complex(64));
  test_herk_helper(complex(128));
}

proc test_syr2k() {
  test_syr2k_helper(real(32));
  test_syr2k_helper(real(64));
  test_syr2k_helper(complex(64));
  test_syr2k_helper(complex(128));
}

proc test_her2k() {
  test_her2k_helper(complex(64));
  test_her2k_helper(complex(128));
}

proc test_trmm() {
  test_trmm_helper(real(32));
  test_trmm_helper(real(64));
  test_trmm_helper(complex(64));
  test_trmm_helper(complex(128));
}

proc test_trsm() {
  test_trsm_helper(real(32));
  test_trsm_helper(real(64));
  test_trsm_helper(complex(64));
  test_trsm_helper(complex(128));
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

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
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

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
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

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
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

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(B);
    rng.fillRandom(C);
    D = C;
    const alpha = rng.getNext(),
          beta = rng.getNext();

    gemm(A[..,0.. #k], B[..,0.. #n], C[..,0.. #n], alpha,beta);
    forall (i,j) in {0.. #m, 0.. #n} do
      D[i,j] = beta*D[i,j]+alpha*(+ reduce (A[i,0.. #k]*B[..,j]));
    var err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }


  writef("%sgemm : %i PASSED, %i FAILED \n", blasPrefix(t), passed, failed);
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

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    makeSymm(A);
    rng.fillRandom(B);
    rng.fillRandom(C);
    var saveC = C;
    D = C;
    const alpha = rng.getNext(),
          beta = rng.getNext();

    symm(A, B, C, alpha, beta, uplo=Uplo.Upper, side=Side.Left);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta);
    var err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    symm(A, B, C, alpha, beta, uplo=Uplo.Lower, side=Side.Left);
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

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    makeSymm(A);
    rng.fillRandom(B);
    rng.fillRandom(C);
    var saveC = C;
    D = C;
    const alpha = rng.getNext(),
          beta = rng.getNext();

    symm(A, B, C, alpha, beta, uplo=Uplo.Upper, side=Side.Right);
    // Do a direct multiplication as a test
    gemm(B,A,D,alpha,beta);
    var err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    symm(A, B, C, alpha, beta, uplo=Uplo.Lower, side=Side.Right);
    // Do a direct multiplication as a test
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

  }
  writef("%s : %i PASSED, %i FAILED \n", name, passed, failed);
}


proc test_hemm_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%shemm".format(blasPrefix(t));

  // Simple tests 1 & 2
  {
    const m = 10 : c_int,
          n = 7 : c_int;
    // Test dgemm -- do this with an array that isn't square
    var A : [{0.. #m, 0.. #m}]t,
        B : [{0.. #m, 0.. #n}]t,
        C : [{0.. #m, 0.. #n}]t,
        D : [{0.. #m, 0.. #n}]t;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    makeHerm(A);
    rng.fillRandom(B);
    rng.fillRandom(C);
    var saveC = C;
    D = C;
    const alpha = rng.getNext(),
          beta = rng.getNext();

    hemm(A, B, C, alpha, beta, uplo=Uplo.Upper, side=Side.Left);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta);
    var err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    hemm(A, B, C, alpha, beta, uplo=Uplo.Lower, side=Side.Left);
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

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    makeHerm(A);
    rng.fillRandom(B);
    rng.fillRandom(C);
    var saveC = C;
    D = C;
    const alpha = rng.getNext(),
          beta = rng.getNext();

    hemm(A, B, C, alpha, beta, uplo=Uplo.Upper, side=Side.Right);
    // Do a direct multiplication as a test
    gemm(B,A,D,alpha,beta);
    var err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    hemm(A, B, C, alpha, beta, uplo=Uplo.Lower, side=Side.Right);
    // Do a direct multiplication as a test
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

  }
  writef("%s : %i PASSED, %i FAILED \n", name, passed, failed);
}

proc test_syrk_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%ssyrk".format(blasPrefix(t));

  // Simple tests 1 & 2
  {
    const n = 10 : c_int;
    // Test dgemm -- do this with an array that isn't square
    var A : [{0.. #n, 0.. #n}]t,
        C : [{0.. #n, 0.. #n}]t,
        D : [{0.. #n, 0.. #n}]t;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    var B = A;
    rng.fillRandom(C);
    makeSymm(C);
    var saveC = C;
    D = C;
    const alpha = rng.getNext(),
          beta = rng.getNext();

    syrk(A, C, alpha, beta, uplo=Uplo.Upper, trans=Op.N);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opB=Op.T);
    zeroTri(C, zeroLow=true);
    zeroTri(D, zeroLow=true);
    var err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    D = saveC;
    syrk(A, C, alpha, beta, uplo=Uplo.Lower, trans=Op.N);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opB=Op.T);
    zeroTri(C, zeroLow=false);
    zeroTri(D, zeroLow=false);
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    D = saveC;
    syrk(A, C, alpha, beta, uplo=Uplo.Upper, trans=Op.T);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opA=Op.T);
    zeroTri(C, zeroLow=true);
    zeroTri(D, zeroLow=true);
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    D = saveC;
    syrk(A, C, alpha, beta, uplo=Uplo.Lower, trans=Op.T);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opA=Op.T);
    zeroTri(C, zeroLow=false);
    zeroTri(D, zeroLow=false);
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  writef("%s : %i PASSED, %i FAILED \n", name, passed, failed);
}

proc test_herk_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sherk".format(blasPrefix(t));

  // Simple tests 1 & 2
  {
    const n = 10 : c_int;
    // Test dgemm -- do this with an array that isn't square
    var A : [{0.. #n, 0.. #n}]t,
        C : [{0.. #n, 0.. #n}]t,
        D : [{0.. #n, 0.. #n}]t;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    var B = A;
    rng.fillRandom(C);
    makeHerm(C);
    var saveC = C;
    D = C;
    // HERK uses real alpha and beta -- just grab the real and imaginary parts
    var tmp = rng.getNext();
    const alpha = tmp.re,
          beta = tmp.im;

    herk(A, C, alpha, beta, uplo=Uplo.Upper, trans=Op.N);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opB=Op.H);
    zeroTri(C, zeroLow=true);
    zeroTri(D, zeroLow=true);
    var err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    D = saveC;
    herk(A, C, alpha, beta, uplo=Uplo.Lower, trans=Op.N);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opB=Op.H);
    zeroTri(C, zeroLow=false);
    zeroTri(D, zeroLow=false);
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    D = saveC;
    herk(A, C, alpha, beta, uplo=Uplo.Upper, trans=Op.H);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opA=Op.H);
    zeroTri(C, zeroLow=true);
    zeroTri(D, zeroLow=true);
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    D = saveC;
    herk(A, C, alpha, beta, uplo=Uplo.Lower, trans=Op.H);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opA=Op.H);
    zeroTri(C, zeroLow=false);
    zeroTri(D, zeroLow=false);
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  writef("%s : %i PASSED, %i FAILED \n", name, passed, failed);
}


proc test_syr2k_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%ssyr2k".format(blasPrefix(t));

  // Simple tests 1 & 2
  {
    const n = 10 : c_int;
    // Test dgemm -- do this with an array that isn't square
    var A : [{0.. #n, 0.. #n}]t,
        B : [{0.. #n, 0.. #n}]t,
        C : [{0.. #n, 0.. #n}]t,
        D : [{0.. #n, 0.. #n}]t;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(B);
    rng.fillRandom(C);
    makeSymm(C);
    var saveC = C;
    D = C;
    const alpha = rng.getNext(),
          beta = rng.getNext();

    var one = 1 : t;
    syr2k(A, B, C, alpha, beta, uplo=Uplo.Upper, trans=Op.N);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opB=Op.T);
    gemm(B,A,D,alpha,one,opB=Op.T);
    zeroTri(C, zeroLow=true);
    zeroTri(D, zeroLow=true);
    var err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    D = saveC;
    syr2k(A, B, C, alpha, beta, uplo=Uplo.Lower, trans=Op.N);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opB=Op.T);
    gemm(B,A,D,alpha,one,opB=Op.T);
    zeroTri(C, zeroLow=false);
    zeroTri(D, zeroLow=false);
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    D = saveC;
    syr2k(A, B, C, alpha, beta, uplo=Uplo.Upper, trans=Op.T);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opA=Op.T);
    gemm(B,A,D,alpha,one,opA=Op.T);
    zeroTri(C, zeroLow=true);
    zeroTri(D, zeroLow=true);
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    D = saveC;
    syr2k(A, B, C, alpha, beta, uplo=Uplo.Lower, trans=Op.T);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opA=Op.T);
    gemm(B,A,D,alpha,one,opA=Op.T);
    zeroTri(C, zeroLow=false);
    zeroTri(D, zeroLow=false);
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  writef("%s : %i PASSED, %i FAILED \n", name, passed, failed);
}

proc test_her2k_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sher2k".format(blasPrefix(t));

  // Simple tests 1 & 2
  {
    const n = 10 : c_int;
    // Test dgemm -- do this with an array that isn't square
    var A : [{0.. #n, 0.. #n}]t,
        B : [{0.. #n, 0.. #n}]t,
        C : [{0.. #n, 0.. #n}]t,
        D : [{0.. #n, 0.. #n}]t;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(B);
    rng.fillRandom(C);
    makeHerm(C);
    var saveC = C;
    D = C;
    const alpha = rng.getNext(),
          calpha = conjg(alpha);
    var tmp = rng.getNext();
    // Beta is real
    const beta = tmp.re;

    var one = 1 : t;
    her2k(A, B, C, alpha, beta, uplo=Uplo.Upper, trans=Op.N);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opB=Op.H);
    gemm(B,A,D,calpha,one,opB=Op.H);
    zeroTri(C, zeroLow=true);
    zeroTri(D, zeroLow=true);
    var err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    D = saveC;
    her2k(A, B, C, alpha, beta, uplo=Uplo.Lower, trans=Op.N);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opB=Op.H);
    gemm(B,A,D,calpha,one,opB=Op.H);
    zeroTri(C, zeroLow=false);
    zeroTri(D, zeroLow=false);
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    D = saveC;
    her2k(A, B, C, alpha, beta, uplo=Uplo.Upper, trans=Op.H);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opA=Op.H);
    gemm(B,A,D,calpha,one,opA=Op.H);
    zeroTri(C, zeroLow=true);
    zeroTri(D, zeroLow=true);
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    C = saveC;
    D = saveC;
    her2k(A, B, C, alpha, beta, uplo=Uplo.Lower, trans=Op.H);
    // Do a direct multiplication as a test
    gemm(A,B,D,alpha,beta,opA=Op.H);
    gemm(B,A,D,calpha,one,opA=Op.H);
    zeroTri(C, zeroLow=false);
    zeroTri(D, zeroLow=false);
    err = max reduce abs(C-D);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  writef("%s : %i PASSED, %i FAILED \n", name, passed, failed);
}

// TODO : There are many possibilities for trmm. We test a few of them, randomly
// chosen. We may need to add more.
proc test_trmm_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%strmm".format(blasPrefix(t));

  // Simple tests 1 & 2
  {
    const n = 10 : c_int;
    // Test dgemm -- do this with an array that isn't square
    var A : [{0.. #n, 0.. #n}]t,
        B : [{0.. #n, 0.. #n}]t,
        C : [{0.. #n, 0.. #n}]t;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(B);
    var saveB = B;
    const alpha = rng.getNext();

    var zero = 0 : t;
    fillRandom(A);
    zeroTri(A, zeroLow=true);
    trmm(A, B, alpha, uplo=Uplo.Upper, trans=Op.N, side=Side.Left);
    // Do a direct multiplication as a test
    gemm(A,saveB,C,alpha,zero);
    var err = max reduce abs(B-C);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    fillRandom(A);
    B = saveB;
    zeroTri(A, zeroLow=false);
    trmm(A, B, alpha, uplo=Uplo.Lower, trans=Op.N, side=Side.Left);
    // Do a direct multiplication as a test
    gemm(A,saveB,C,alpha,zero);
    err = max reduce abs(B-C);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    fillRandom(A);
    B = saveB;
    zeroTri(A, zeroLow=true);
    trmm(A, B, alpha, uplo=Uplo.Upper, trans=Op.N, side=Side.Right);
    // Do a direct multiplication as a test
    gemm(saveB,A,C,alpha,zero);
    err = max reduce abs(B-C);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    fillRandom(A);
    B = saveB;
    zeroTri(A, zeroLow=true);
    trmm(A, B, alpha, uplo=Uplo.Upper, trans=Op.T, side=Side.Right);
    // Do a direct multiplication as a test
    gemm(saveB,A,C,alpha,zero,opB=Op.T);
    err = max reduce abs(B-C);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  writef("%s : %i PASSED, %i FAILED \n", name, passed, failed);
}

// TODO : There are many possibilities for trsm. We test a few of them, randomly
// chosen. We may need to add more.
// We need to be more careful with setting up the problem here; we don't want
// poorly conditioned solutions. We try to get around this by regularizing the
// A matrix with a relatively large diagonal term.
proc test_trsm_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%strsm".format(blasPrefix(t));

  // Simple tests 1 & 2
  {
    const n = 10 : c_int;
    // Test dgemm -- do this with an array that isn't square
    var A : [{0.. #n, 0.. #n}]t,
        B : [{0.. #n, 0.. #n}]t,
        C : [{0.. #n, 0.. #n}]t,
        Id : [{0.. #n, 0.. #n}]t;

    makeUnit(Id, 5.0);

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(B);
    var saveB = B;
    const alpha = rng.getNext();

    var zero = 0 : t,
        one = 1 : t;
    fillRandom(A);
    A += Id;
    zeroTri(A, zeroLow=true);
    trsm(A, B, alpha, uplo=Uplo.Upper, trans=Op.N, side=Side.Left);
    // Do a direct multiplication as a test
    C = zero;
    gemm(A,B,C,one,zero);
    var err = max reduce abs(alpha*saveB-C);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    fillRandom(A);
    A += Id;
    B = saveB;
    zeroTri(A, zeroLow=false);
    trsm(A, B, alpha, uplo=Uplo.Lower, trans=Op.N, side=Side.Left);
    // Do a direct multiplication as a test
    C = zero;
    gemm(A,B,C,one,zero);
    err = max reduce abs(alpha*saveB-C);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    fillRandom(A);
    A += Id;
    B = saveB;
    zeroTri(A, zeroLow=true);
    trsm(A, B, alpha, uplo=Uplo.Upper, trans=Op.N, side=Side.Right);
    // Do a direct multiplication as a test
    C = zero;
    gemm(B,A,C,one,zero);
    err = max reduce abs(alpha*saveB-C);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    fillRandom(A);
    A += Id;
    B = saveB;
    zeroTri(A, zeroLow=true);
    trsm(A, B, alpha, uplo=Uplo.Upper, trans=Op.T, side=Side.Right);
    // Do a direct multiplication as a test
    C = zero;
    gemm(B,A,C,one,zero,opB=Op.T);
    err = max reduce abs(alpha*saveB-C);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  writef("%s : %i PASSED, %i FAILED \n", name, passed, failed);
}
