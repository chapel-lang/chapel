use Random;
use BLAS;
use blas_helpers;
use IO;

/* BLAS 2 Testing TODO:
    - [ ] Banded matrices (commented out)
      - [ ] blas_helpers functions (commented out in blas_helpers.chpl)
    - [ ] Packed matrices (commented out)
      - [ ] blas_helpers functions (commented out in blas_helpers.chpl)
 */


proc main() {
    //test_gbmv(); // bandMatrix
    test_gemv();
    test_ger();
    test_gerc();
    test_geru();
    //test_hbmv(); // bandMatrix;
    test_hemv();
    test_her();
    test_her2();
    //test_hpmv(); // bandMatrix
    //test_hpr();  // packedMatrix
    //test_hpr2(); // packedMatrix
    //test_sbmv(); // bandMatrix
    //test_spmv(); // packedMatrix
    //test_spr();  // packedMatrix
    //test_spr2(); // packedMatrix
    test_symv();
    test_syr();
    test_syr2();
    //test_tbmv(); // bandMatrix
    //test_tbsv(); // bandMatrix
    //test_tpmv(); // packedMatrix
    //test_tpsv(); // packedMatrix
    test_trmv();
    test_trsv();
}


//
// Tests
//

proc test_gbmv() {
    test_gbmv_helper(real(32));
    test_gbmv_helper(real(64));
    test_gbmv_helper(complex(64));
    test_gbmv_helper(complex(128));
}


proc test_gemv() {
    test_gemv_helper(real(32));
    test_gemv_helper(real(64));
    test_gemv_helper(complex(64));
    test_gemv_helper(complex(128));
}


proc test_ger() {
    test_ger_helper(real(32));
    test_ger_helper(real(64));
}


proc test_gerc() {
    test_gerc_helper(complex(64));
    test_gerc_helper(complex(128));
}


proc test_geru() {
    test_geru_helper(complex(64));
    test_geru_helper(complex(128));
}


proc test_hbmv() {
    test_hbmv_helper(complex(64));
    test_hbmv_helper(complex(128));
}


proc test_hemv() {
    test_hemv_helper(complex(64));
    test_hemv_helper(complex(128));
}


proc test_her() {
    test_her_helper(complex(64));
    test_her_helper(complex(128));
}


proc test_her2() {
    test_her2_helper(complex(64));
    test_her2_helper(complex(128));
}


proc test_hpmv() {
    test_hpmv_helper(complex(64));
    test_hpmv_helper(complex(128));
}


proc test_hpr() {
    test_hpr_helper(complex(64));
    test_hpr_helper(complex(128));
}


proc test_hpr2() {
    test_hpr2_helper(complex(64));
    test_hpr2_helper(complex(128));
}


proc test_sbmv() {
    test_sbmv_helper(real(32));
    test_sbmv_helper(real(64));
}


proc test_spmv() {
    test_spmv_helper(real(32));
    test_spmv_helper(real(64));
}


proc test_spr() {
    test_spr_helper(real(32));
    test_spr_helper(real(64));
}


proc test_spr2() {
    test_spr2_helper(real(32));
    test_spr2_helper(real(64));
}


proc test_symv() {
    test_symv_helper(real(32));
    test_symv_helper(real(64));
}


proc test_syr() {
    test_syr_helper(real(32));
    test_syr_helper(real(64));
}


proc test_syr2() {
    test_syr2_helper(real(32));
    test_syr2_helper(real(64));
}


proc test_tbmv() {
    test_tbmv_helper(real(32));
    test_tbmv_helper(real(64));
    test_tbmv_helper(complex(64));
    test_tbmv_helper(complex(128));
}


proc test_tbsv() {
    test_tbsv_helper(real(32));
    test_tbsv_helper(real(64));
    test_tbsv_helper(complex(64));
    test_tbsv_helper(complex(128));
}


proc test_trmv() {
    test_trmv_helper(real(32));
    test_trmv_helper(real(64));
    test_trmv_helper(complex(64));
    test_trmv_helper(complex(128));
}


proc test_trsv() {
    test_trsv_helper(real(32));
    test_trsv_helper(real(64));
    test_trsv_helper(complex(64));
    test_trsv_helper(complex(128));
}


//
// Helpers
//

// TODO -- work in progress
proc test_gbmv_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sgbmv".format(blasPrefix(t));

  // Simple test
  {
    const m = 6,
          n = 6;
    // Square
    var A : [{0.. #m, 0.. #n}]t,
        X : [{0.. #n}]t,
        Y : [{0.. #m}]t,
        R : [{0.. #m}]t; // Result

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var alpha = 1: t, //rng.getNext();
        beta = 2: t;//rng.getNext();
    //var rngint = createRandomStream(eltType=int,algorithm=RNG.PCG);
    //var kl = rng.getNext(0, min(m, n)) ,
    //    ku = rng.getNext(0, min(m, n)) ;
    var kl = 3,
        ku = 3;

    //rng.fillRandom(A);
    //rng.fillRandom(X);
    //rng.fillRandom(Y);
    X = 2: t;
    Y = 2: t;
    A = 2: t;
    makeBand(A, kl, ku);

    // Band-diagonal representation
    var AB = bandArray(A, kl, ku, order=Order.Row);

    // Compute Result vector
    for i in R.domain do R[i] = beta*Y[i] + alpha*(+ reduce (A[i,..]*X[..]));
    gbmv(AB, X, Y, alpha, beta, order=Order.Row, kl=kl, ku=ku);

    var err = max reduce abs(Y-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}


proc test_gemv_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sgemv".format(blasPrefix(t));

  // Simple test
  {
    const m = 10 : c_int;

    // Square
    var A : [{0.. #m, 0.. #m}]t,
        X : [{0.. #m}]t,
        Y : [{0.. #m}]t,
        R : [{0.. #m}]t; // Result


    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    const alpha = rng.getNext();
    const beta = rng.getNext();

    // Compute Result vector
    for i in R.domain do R[i] = beta*Y[i] + alpha*(+ reduce (A[i,..]*X[..]));

    gemv(A, X, Y, alpha, beta);

    var err = max reduce abs(Y-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Try transposing A
  {
    const m = 10 : c_int;

    // Square
    var A : [{0.. #m, 0.. #m}]t,
        X : [{0.. #m}]t,
        Y : [{0.. #m}]t,
        R : [{0.. #m}]t; // Result


    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    const alpha = rng.getNext();
    const beta = rng.getNext();

    // Compute Result vector
    for i in R.domain do R[i] = beta*Y[i] + alpha*(+ reduce (A[..,i]*X[..]));

    gemv(A, X, Y, alpha, beta, opA=Op.T);

    var err = max reduce abs(Y-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  // Try Hermitian conjugate of A
  {
    const m = 10 : c_int;

    // Square
    var A : [{0.. #m, 0.. #m}]t,
        X : [{0.. #m}]t,
        Y : [{0.. #m}]t,
        R : [{0.. #m}]t; // Result


    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    const alpha = rng.getNext();
    const beta = rng.getNext();

    // Compute Result vector
    for i in R.domain do
      R[i] = beta*Y[i] + alpha*(+ reduce (conjg(A[..,i])*X[..]));

    gemv(A, X, Y, alpha, beta, opA=Op.H);

    var err = max reduce abs(Y-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Test leading dimension of array
  {
    const m = 10 : c_int,
          n = 7 : c_int,
          ld = 20 : c_int;

    // non-square matrix
    var A : [{0.. #ld, 0.. #ld}]t,
        X : [{0.. #n}]t,
        Y : [{0.. #m}]t,
        R : [{0.. #m}]t; // Result


    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    const alpha = rng.getNext();
    const beta = rng.getNext();

    // Compute Result vector
    for i in R.domain do R[i] = beta*Y[i] + alpha*(+ reduce (A[i+2 ,0.. #n]*X[..]));

    gemv(A[2..#m, 0.. #n] , X, Y, alpha, beta);

    var err = max reduce abs(Y-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
 }

  printErrors(name, passed, failed, tests);
}


proc test_ger_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sger".format(blasPrefix(t));

  // Simple test
  {

    // Square case
    const m = 10 : c_int,
          n = 10 : c_int;

    var A : [{0.. #m, 0.. #n}] t,
        X : [{0.. #m}]t,
        Y : [{0.. #n}]t,
        R : [{0.. #m, 0..#n}] t; // Result


    // Populate values
    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);
    const alpha = rng.getNext();

    // Precompute result
    for (i,j) in R.domain do R[i, j] = alpha*X[i]*Y[j] + A[i, j];

    ger(A, X, Y, alpha);

    var err = max reduce abs(A-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Test leading dimension of array
  {

    const m = 10 : c_int,
          n = 7 : c_int,
          ld = 20 : c_int;

    // non-square matrix
    var A : [{0.. #m, 0.. #ld}]t,
        X : [{0.. #m}]t,
        Y : [{0.. #n}]t,
        R : [{0.. #m, 0..#n}] t; // Result


    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    const alpha = rng.getNext();

    for (i,j) in R.domain do R[i, j] = alpha*X[i]*Y[j] + A[i, j];

    ger(A[.., 0.. #n], X, Y, alpha);

    var err = max reduce abs(A[.., 0..#n]-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
 }

  printErrors(name, passed, failed, tests);
}


proc test_gerc_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sgerc".format(blasPrefix(t));

  // Simple test
  {
    // Square case
    const m = 10 : c_int,
          n = 10 : c_int;

    var A : [{0.. #m, 0.. #n}] t,
        X : [{0.. #m}]t,
        Y : [{0.. #n}]t,
        R : [{0.. #m, 0..#n}] t; // Result


    // Populate values
    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);
    var alpha = rng.getNext();

    // Precompute result
    for (i,j) in R.domain do R[i, j] = alpha*X[i]*conjg(Y[j]) + A[i, j];

    gerc(A, X, Y, alpha);

    var err = max reduce abs(A-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Test leading dimension of array
  {

    const m = 10 : c_int,
          n = 7 : c_int,
          ld = 20 : c_int;

    // non-square matrix
    var A : [{0.. #m, 0.. #ld}]t,
        X : [{0.. #m}]t,
        Y : [{0.. #n}]t,
        R : [{0.. #m, 0..#n}] t; // Result


    // Populate values
    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    var alpha = rng.getNext();

    // Precompute result
    for (i,j) in R.domain do R[i, j] = alpha*X[i]*conjg(Y[j]) + A[i, j];

    gerc(A[.., 0.. #n], X, Y, alpha);

    var err = max reduce abs(A[.., 0.. #n] - R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}


proc test_geru_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sgeru".format(blasPrefix(t));
  // Simple test
  {
    // Square case
    const m = 10 : c_int,
          n = 10 : c_int;

    var A : [{0.. #m, 0.. #n}] t,
        X : [{0.. #m}]t,
        Y : [{0.. #n}]t,
        R : [{0.. #m, 0..#n}] t; // Result


    // Populate values
    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);
    var alpha = rng.getNext();

    // Precompute result
    for (i,j) in R.domain do R[i, j] = alpha*X[i]*Y[j] + A[i, j];

    geru(A, X, Y, alpha);

    var err = max reduce abs(A-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Test leading dimension of array
  {
    const m = 10 : c_int,
          n = 7 : c_int,
          ld = 20 : c_int;

    // non-square matrix
    var A : [{0.. #m, 0.. #ld}]t,
        X : [{0.. #m}]t,
        Y : [{0.. #n}]t,
        R : [{0.. #m, 0..#n}] t; // Result


    // Populate values
    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    var alpha = rng.getNext();

    // Precompute result
    for (i,j) in R.domain do R[i, j] = alpha*X[i]*Y[j] + A[i, j];

    geru(A[.., 0.. #n], X, Y, alpha);

    var err = max reduce abs(A[.., 0.. #n] - R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  printErrors(name, passed, failed, tests);
}


// TODO
proc test_hbmv_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%shbmv".format(blasPrefix(t));
  // Simple test
  {
  }
  printErrors(name, passed, failed, tests);
}


proc test_hemv_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%shemv".format(blasPrefix(t));
  // Simple test
  {
    const m = 10 : c_int;

    var A : [0.. #m, 0.. #m] t,
        X : [0.. #m] t,
        Y : [0.. #m] t,
        R : [0.. #m] t; // Result


    // Populate values
    var rng = createRandomStream(eltType=t, algorithm=RNG.PCG);

    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    var alpha = rng.getNext(),
        beta = rng.getNext();

    makeHerm(A);

    // Precompute result
    for i in R.domain do R[i] = + reduce(alpha*A[i,..]*X[..]) + beta*Y[i];

    hemv(A, X, Y, alpha, beta);

    var err = max reduce abs(Y-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Try Uplo.Lower
  {
    //const m = 10 : c_int;
    const m = 2 : c_int;

    var A : [0.. #m, 0.. #m] t,
        X : [0.. #m] t,
        Y : [0.. #m] t,
        R : [0.. #m] t; // Result


    // Populate values
    var rng = createRandomStream(eltType=t, algorithm=RNG.PCG);

    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    var alpha = rng.getNext(),
        beta = rng.getNext();

    makeHerm(A);

    // Precompute result
    for i in R.domain do R[i] = + reduce(alpha*A[i,..]*X[..]) + beta*Y[i];

    hemv(A, X, Y, alpha, beta, uplo=Uplo.Lower);

    var err = max reduce abs(Y-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Test leading dimension of array
  {
    const m = 10 : c_int;
    const ld = 12;

    var A : [0.. #m, 0.. #ld] t,
        X : [0.. #m] t,
        Y : [0.. #m] t,
        R : [0.. #m] t; // Result


    // Populate values
    var rng = createRandomStream(eltType=t, algorithm=RNG.PCG);

    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    var alpha = rng.getNext(),
        beta = rng.getNext();

    makeHerm(A[.., 0..#m]);

    // Precompute result

    for i in R.domain do R[i] = + reduce(alpha*A[i,0..#m]*X[..]) + beta*Y[i];

    hemv(A[.., 0..#m], X, Y, alpha, beta);

    var err = max reduce abs(Y-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}


proc test_her_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sher".format(blasPrefix(t));
  // Simple test
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}]t,
        R : [{0.. #m, 0..#m}] t; // Result

    var alphacomplex = rng.getNext(),
        alpha = alphacomplex.re;
    rng.fillRandom(A);
    rng.fillRandom(X);

    makeHerm(A);

    // Precompute result
    for (i,j) in R.domain do R[i, j] = alpha*X[i]*conjg(X[j]) + A[i, j];

    // Resulting 'A' is only stored in upper triangular array
    zeroTri(A);
    zeroTri(R);

    her(A, X, alpha);

    var err = max reduce abs(A-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Try Uplo.Lower
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}]t,
        R : [{0.. #m, 0..#m}] t; // Result

    var alphacomplex = rng.getNext(),
        alpha = alphacomplex.re;
    rng.fillRandom(A);
    rng.fillRandom(X);

    makeHerm(A);

    // Precompute result
    for (i,j) in R.domain do R[i, j] = alpha*X[i]*conjg(X[j]) + A[i, j];

    // Resulting 'A' is only stored in upper triangular array
    zeroTri(A, uplo=Uplo.Lower);
    zeroTri(R, uplo=Uplo.Lower);

    her(A, X, alpha, uplo=Uplo.Lower);

    var err = max reduce abs(A-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Test leading dimension of array
  {
    const m = 10 : c_int;
    const ld = 20;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #ld}] t,
        X : [{0.. #m}]t,
        R : [{0.. #m, 0..#m}] t; // Result

    var alphacomplex = rng.getNext(),
        alpha = alphacomplex.re;
    rng.fillRandom(A);
    rng.fillRandom(X);

    makeHerm(A[.., 0..#m]);

    // Precompute result
    for (i,j) in R.domain do R[i, j] = alpha*X[i]*conjg(X[j]) + A[i, j];

    // Resulting 'A' is only stored in upper triangular array
    zeroTri(A[.., 0..#m]);
    zeroTri(R);

    her(A[.., 0..#m], X, alpha);

    var err = max reduce abs(A[.., 0..#m]-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}


proc test_her2_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sher2".format(blasPrefix(t));
  // Simple test
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        Y : [{0.. #m}] t,
        R : [{0.. #m, 0..#m}] t; // Result

    var alpha = rng.getNext();
    rng.fillRandom(A);
    rng.fillRandom(X);

    makeHerm(A);

    // Precompute result
    for (i, j) in R.domain do
      R[i, j] = alpha*X[i]*conjg(Y[j]) + conjg(alpha)*Y[i]*conjg(Y[j]) + A[i, j];

    // A result is only stored in upper triangular array
    zeroTri(A);
    zeroTri(R);

    her2(A, X, Y, alpha);

    var err = max reduce abs(A-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Try Uplo.Lower
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        Y : [{0.. #m}] t,
        R : [{0.. #m, 0..#m}] t; // Result

    var alpha = rng.getNext();
    rng.fillRandom(A);
    rng.fillRandom(X);

    makeHerm(A);

    // Precompute result
    for (i, j) in R.domain do
      R[i, j] = alpha*X[i]*conjg(Y[j]) + conjg(alpha)*Y[i]*conjg(Y[j]) + A[i, j];

    // A result is only stored in upper triangular array
    zeroTri(A, uplo=Uplo.Lower);
    zeroTri(R, uplo=Uplo.Lower);

    her2(A, X, Y, alpha, uplo=Uplo.Lower);

    var err = max reduce abs(A-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Test leading dimension of array
  {
    const m = 10 : c_int;
    const ld = 20;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0..#m, 0..#ld}] t,
        X : [{0..#m}] t,
        Y : [{0..#m}] t,
        R : [{0..#m, 0..#m}] t; // Result

    var alpha = rng.getNext();
    rng.fillRandom(A);
    rng.fillRandom(X);

    makeHerm(A[.., 0..#m]);

    // Precompute result
    for (i, j) in R.domain do
      R[i, j] = alpha*X[i]*conjg(Y[j]) + conjg(alpha)*Y[i]*conjg(Y[j]) + A[i, j];

    // A result is only stored in upper triangular array
    zeroTri(A[.., 0..#m]);
    zeroTri(R);

    her2(A[.., 0..#m], X, Y, alpha);

    var err = max reduce abs(A[.., 0..#m]-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}


// TODO
proc test_hpmv_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%shpmv".format(blasPrefix(t));
  // Simple test
  {
  }
  printErrors(name, passed, failed, tests);
}


// TODO
proc test_hpr_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%shpr".format(blasPrefix(t));
  // Simple test
  {
  }
  printErrors(name, passed, failed, tests);
}


// TODO
proc test_hpr2_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%shpr2".format(blasPrefix(t));
  // Simple test
  {
  }
  printErrors(name, passed, failed, tests);
}


// TODO
proc test_sbmv_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%ssbmv".format(blasPrefix(t));
  // Simple test
  {
  }
  printErrors(name, passed, failed, tests);
}


// TODO
proc test_spmv_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sspmv".format(blasPrefix(t));
  // Simple test
  {
  }
  printErrors(name, passed, failed, tests);
}


// TODO
proc test_spr_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sspr".format(blasPrefix(t));
  // Simple test
  {
  }
  printErrors(name, passed, failed, tests);
}


// TODO
proc test_spr2_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sspr2".format(blasPrefix(t));
  // Simple test
  {
  }
  printErrors(name, passed, failed, tests);
}


proc test_symv_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%ssymv".format(blasPrefix(t));
  // Simple test
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        Y : [{0.. #m}] t,
        R : [{0.. #m}] t; // Result

    var alpha = rng.getNext(),
        beta = rng.getNext();

    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    makeSymm(A);

    // Precompute result
    for i in R.domain {
      R[i] = beta*Y[i] + alpha*(+ reduce (A[i,..]*X[..]));
    }

    // A result is only stored in upper triangular array
    zeroTri(A);

    symv(A, X, Y, alpha, beta);

    var err = max reduce abs(Y-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Try Uplo.Lower
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        Y : [{0.. #m}] t,
        R : [{0.. #m}] t; // Result

    var alpha = rng.getNext(),
        beta = rng.getNext();

    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    makeSymm(A);

    // Precompute result
    for i in R.domain do R[i] = beta*Y[i] + alpha*(+ reduce (A[i,..]*X[..]));

    // A result is only stored in upper triangular array
    zeroTri(A, Uplo.Lower);

    symv(A, X, Y, alpha, beta, uplo=Uplo.Lower);

    var err = max reduce abs(Y-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Test leading dimension of array
  {
    const m = 10 : c_int;
    const ld = 20: c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #ld}] t,
        X : [{0.. #m}] t,
        Y : [{0.. #m}] t,
        R : [{0.. #m}] t; // Result

    var alpha = rng.getNext(),
        beta = rng.getNext();

    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    makeSymm(A[.., 0..#m]);

    // Precompute result
    for i in R.domain do R[i] = beta*Y[i] + alpha*(+ reduce (A[i,0..#m]*X[..]));

    // A result is only stored in upper triangular array
    zeroTri(A[.., 0..#m]);

    symv(A[.., 0..#m], X, Y, alpha, beta);

    var err = max reduce abs(Y-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}


proc test_syr_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%ssyr".format(blasPrefix(t));
  // Simple test
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        R : [A.domain] t; // Result

    var alpha = rng.getNext();

    rng.fillRandom(A);
    rng.fillRandom(X);

    makeSymm(A);

    for (i, j) in R.domain do R[i, j] = alpha*X[i]*conjg(X[j]) + A[i, j];

    // A result is only stored in upper triangular array
    zeroTri(R);
    zeroTri(A);

    syr(A, X, alpha);

    var err = max reduce abs(A-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Try Uplo.Lower
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        R : [A.domain] t; // Result

    var alpha = rng.getNext();

    rng.fillRandom(A);
    rng.fillRandom(X);

    makeSymm(A);

    for (i, j) in R.domain do R[i, j] = alpha*X[i]*conjg(X[j]) + A[i, j];

    // A result is only stored in upper triangular array
    zeroTri(R, Uplo.Lower);
    zeroTri(A, Uplo.Lower);

    syr(A, X, alpha, uplo=Uplo.Lower);

    var err = max reduce abs(A-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Test leading dimension of array
  {
    const m = 10 : c_int;
    const ld = 20;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0..#m, 0..#ld}] t,
        X : [{0.. #m}] t,
        R : [0..#m, 0..#m] t; // Result

    var alpha = rng.getNext();

    rng.fillRandom(A);
    rng.fillRandom(X);

    makeSymm(A[.., 0..#m]);

    for (i, j) in R.domain do R[i, j] = alpha*X[i]*conjg(X[j]) + A[i, j];

    // A result is only stored in upper triangular array
    zeroTri(R);
    zeroTri(A[.., 0..#m]);

    syr(A[.., 0..#m], X, alpha);

    var err = max reduce abs(A[.., 0..#m]-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}


proc test_syr2_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%ssyr2".format(blasPrefix(t));
  // Simple test
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        Y : [{0.. #m}] t,
        R : [{0..#m, 0..#m}] t; // Result

    var alpha = rng.getNext();

    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    makeSymm(A);

    for (i, j) in R.domain do
        R[i, j] = alpha*X[i]*conjg(Y[j]) + conjg(alpha)*Y[i]*conjg(X[j]) + A[i,j];

    // A result is only stored in upper triangular array
    zeroTri(R);
    zeroTri(A);

    syr2(A, X, Y, alpha);

    var err = max reduce abs(A-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Try Uplo.Lower
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        Y : [{0.. #m}] t,
        R : [{0..#m, 0..#m}] t; // Result

    var alpha = rng.getNext();

    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    makeSymm(A);

    for (i, j) in R.domain do
        R[i, j] = alpha*X[i]*conjg(Y[j]) + conjg(alpha)*Y[i]*conjg(X[j]) + A[i,j];

    // A result is only stored in upper triangular array
    zeroTri(R, Uplo.Lower);
    zeroTri(A, Uplo.Lower);

    syr2(A, X, Y, alpha, uplo=Uplo.Lower);

    var err = max reduce abs(A-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Test leading dimension of array
  {
    const m = 10 : c_int;
    const ld = 20;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #ld}] t,
        X : [{0.. #m}] t,
        Y : [{0.. #m}] t,
        R : [{0..#m, 0..#m}] t; // Result

    var alpha = rng.getNext();

    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    makeSymm(A[.., 0..#m]);

    for (i, j) in R.domain do
        R[i, j] = alpha*X[i]*conjg(Y[j]) + conjg(alpha)*Y[i]*conjg(X[j]) + A[i,j];

    // A result is only stored in upper triangular array
    zeroTri(R);
    zeroTri(A[.., 0..#m]);

    syr2(A[.., 0..#m], X, Y, alpha);

    var err = max reduce abs(A[.., 0..#m]-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}


// TODO
proc test_tbmv_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%stbmv".format(blasPrefix(t));
  // Simple test
  {
    const m = 5 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        R : [X.domain] t; // Result

    rng.fillRandom(A);
    rng.fillRandom(X);

    const k = m-1: c_int;

    // Triangulate Matrix
    zeroTri(A);

    // Create band matrix
    var AB = bandArrayTriangular(A, k, uplo=Uplo.Upper);

    // Precompute result
    for i in R.domain {
      R[i] = + reduce (A[i,..]*X[..]);
    }

    tbmv(AB, X, k);

    var err = max reduce abs(X-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}


// TODO
proc test_tbsv_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%stbsv".format(blasPrefix(t));
  // Simple test
  {
  }
  printErrors(name, passed, failed, tests);
}


proc test_trmv_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%strmv".format(blasPrefix(t));
  // Simple test
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        R : [X.domain] t; // Result

    rng.fillRandom(A);
    rng.fillRandom(X);

    // A result is only stored in upper triangular array
    zeroTri(A);

    for i in R.domain do R[i] = + reduce (A[i, ..]*X[..]);

    trmv(A, X);

    var err = max reduce abs(X-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Try trans=Op.T
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        R : [X.domain] t; // Result

    rng.fillRandom(A);
    rng.fillRandom(X);

    // A result is only stored in upper triangular array
    zeroTri(A);

    for i in R.domain do R[i] = + reduce (A[.., i]*X[..]);

    trmv(A, X, trans=Op.T);

    var err = max reduce abs(X-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Try uplo=Uplo.Lower
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        R : [X.domain] t; // Result

    rng.fillRandom(A);
    rng.fillRandom(X);

    // A result is only stored in upper triangular array
    zeroTri(A, Uplo.Lower);

    for i in R.domain do R[i] = + reduce (A[i, ..]*X[..]);

    trmv(A, X, uplo=Uplo.Lower);

    var err = max reduce abs(X-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Try diag=Diag.Unit
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        R : [X.domain] t; // Result

    rng.fillRandom(A);
    rng.fillRandom(X);

    // A result is only stored in upper triangular array
    zeroTri(A);

    // Make unit triangular
    for (i, j) in A.domain do
      if i == j then A[i, j] = 1 :t;

    for i in R.domain do R[i] = + reduce (A[i, ..]*X[..]);

    trmv(A, X, diag=Diag.Unit);

    var err = max reduce abs(X-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Test leading dimension of array
  {
    const m = 10 : c_int;
    const ld = 20;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #ld}] t,
        X : [{0.. #m}] t,
        R : [X.domain] t; // Result

    rng.fillRandom(A);
    rng.fillRandom(X);

    // A result is only stored in upper triangular array
    zeroTri(A[.., 0..#m]);

    for i in R.domain do R[i] = + reduce (A[i, 0..#m]*X[..]);

    trmv(A[.., 0..#m], X);

    var err = max reduce abs(X-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  printErrors(name, passed, failed, tests);
}


proc test_trsv_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%strsv".format(blasPrefix(t));
  // Simple test
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        R : [X.domain] t; // Result


    makeRandomInvertible(A);
    rng.fillRandom(X);

    // Make A a triangular matrix
    zeroTri(A);

    // Store X
    const Xsave = X;

    trsv(A, X);

    for i in R.domain do R[i] = + reduce(A[i, ..]*X[..]);

    var err = max reduce abs(Xsave-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Try trans=Op.T
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        R : [X.domain] t; // Result


    makeRandomInvertible(A);
    rng.fillRandom(X);

    // Make A a triangular matrix
    zeroTri(A);

    // Store X
    const Xsave = X;

    trsv(A, X, trans=Op.T);

    for i in R.domain do R[i] = + reduce(A[.., i]*X[..]);

    var err = max reduce abs(Xsave-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Try uplo=Uplo.Lower
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        R : [X.domain] t; // Result


    makeRandomInvertible(A);
    rng.fillRandom(X);

    // Make A a triangular matrix
    zeroTri(A, Uplo.Lower);

    // Store X
    const Xsave = X;

    trsv(A, X, uplo=Uplo.Lower);

    for i in R.domain do R[i] = + reduce(A[i, ..]*X[..]);

    var err = max reduce abs(Xsave-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Try diag=Diag.Unit
  {
    const m = 10 : c_int;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #m}] t,
        X : [{0.. #m}] t,
        R : [X.domain] t; // Result


    makeRandomInvertible(A);
    rng.fillRandom(X);

    // Make A a triangular matrix
    zeroTri(A);

    // Make A unit triangular
    for (i, j) in A.domain do
      if i == j then A[i, j] = 1 :t;

    // Store X
    const Xsave = X;

    trsv(A, X, diag=Diag.Unit);

    for i in R.domain do R[i] = + reduce(A[i, ..]*X[..]);

    var err = max reduce abs(Xsave-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Test leading dimension of array
  {
    const m = 10 : c_int;
    const ld = 20;

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);

    var A : [{0.. #m, 0.. #ld}] t,
        X : [{0.. #m}] t,
        R : [X.domain] t; // Result


    makeRandomInvertible(A[.., 0..#m]);
    rng.fillRandom(X);

    // Make A a triangular matrix
    zeroTri(A[.., 0..#m]);

    // Store X
    const Xsave = X;

    trsv(A[.., 0..#m], X);

    for i in R.domain do R[i] = + reduce(A[i, 0..#m]*X[..]);

    var err = max reduce abs(Xsave-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  printErrors(name, passed, failed, tests);
}

