use Time;
use Math;
use LinearAlgebra;
use Random;

config var m=4,iters=10,correctness=false;

config type eltType = real;

const nbytes = numBytes(eltType);


proc test_expm() {
  const Space = {0..<m, 0..<m};
  var t: Timer;

  if !correctness {
    writeln('=========================================');
    writeln('  Matrix Exponential Performance Test');
    writeln('=========================================');
    writeln('iters : ', iters);
    writeln('m     : ', m);
    writeln('numLocales     : ', numLocales);
    writeln('MB    : ', (nbytes*m*m) / 10**6);
    writeln();
  }

  {
    // Pade3, Pade5, Pade7, Pade9, Pade13 cases

    var b = [0.01, 0.1, 0.5, 1.0, 10.0];
    const pade = [3, 5, 7, 9, 13];

    for i in b.domain do {
      var X: [Space] eltType;
      setDiag(X, 0, 1);
      var Y = X * b[i];

      var M: [X.domain] eltType;

      for 1..iters {
        t.start();
        M = expm(Y);
        t.stop();
      }

      if !correctness then
        writeln('LinearAlgebra.expm_pade' + (pade[i]: string)
                              + ': ' + ((t.elapsed()/iters): string));
      else
        writeln(M);

      t.clear();
    }
    writeln("");
  }



  {
    // Input Matrices of Format
    // [1.0 10^n 0.0]
    // [0.0 -1.0 0.0]
    // [0.0  0.0 1.0]

    var b = [1.0, 10.0, 100.0, 1000.0, 10000.0, 100000.0,
                                  1000000.0, 10000000.0, 100000000.0];

    for i in b.domain do {
      var X: [Space] eltType;
      setDiag(X, 0, 1);
      X(1,1) = -1.0;
      X(0,1) = b[i];

      var M: [X.domain] eltType;

      for 1..iters {
        t.start();
        M = expm(X);
        t.stop();
      }

      if !correctness then
        writeln('LinearAlgebra.expm_10pow' + (i: string)
                              + ": " + ((t.elapsed()/iters): string));
      else
        writeln(M);

      t.clear();
    }
    writeln("");
  }


  {
    // Input Matrix of Format
    // [ 0.0  1.0  0.0]
    // [-1.0  0.0  1.0]
    // [ 0.0 -1.0  0.0]

    var X: [Space] eltType;
    setDiag(X, 0, 0.0);
    setDiag(X, 1, 1.0);
    setDiag(X, -1, -1.0);

    var M: [X.domain] eltType;

    for 1..iters {
      t.start();
      M = expm(X);
      t.stop();
    }

    if !correctness then
      writeln('LinearAlgebra.expm_gradient: ' + ((t.elapsed()/iters): string));
    else
      writeln(M);
    writeln("");

    t.clear();
  }


  {
    // Input Matrix of Format
    // [-2.0  1.0  0.0]
    // [-1.0 -2.0  1.0]
    // [ 0.0 -1.0 -2.0]

    var X: [Space] eltType;
    setDiag(X, 0, -2.0);
    setDiag(X, 1, 1.0);
    setDiag(X, -1, -1.0);

    var M: [X.domain] eltType;

    for 1..iters {
      t.start();
      M = expm(X);
      t.stop();
    }

    if !correctness then
      writeln('LinearAlgebra.expm_edge: ' + ((t.elapsed()/iters): string));
    else
      writeln(M);
    writeln("");

    t.clear();
  }

  {
    // Sparse Input Matrix
    // 10% Filled.

    var r = new owned PCGRandomStream(int);

    var X: [Space] eltType;

    for i in 0..<m {
      X(r.getNext(0,m-1), i) = 1.0;
    }

    var M: [X.domain] eltType;

    for 1..iters {
      t.start();
      M = expm(X);
      t.stop();
    }

    if !correctness then
      writeln('LinearAlgebra.expm_sparse: ' + ((t.elapsed()/iters): string));
    else
      writeln("");
    writeln("");

    t.clear();
  }

  {
    // Dense Input Matrix
    // 90% Filled.

    var r = new owned PCGRandomStream(int);

    var X: [Space] eltType;
    X = X + 1.0;

    for i in 0..<m {
      X(r.getNext(0,m-1), i) = 0.0;
    }

    var M: [X.domain] eltType;

    for 1..iters {
      t.start();
      M = expm(X);
      t.stop();
    }

    if !correctness then
      writeln('LinearAlgebra.expm_dense: ' + ((t.elapsed()/iters): string));
    else
      writeln("");
    writeln("");

    t.clear();
  }

  {
    // Testing eltType=Complex: Input Matrix of Format
    // [ 1.0 + 1i 1.0       0.0     ]
    // [-1.0      1.0 + 1i  1.0     ]
    // [ 0.0     -1.0       1.0 + 1i]

    var X: [Space] complex;
    setDiag(X, 0, 1.0 + 1i);
    setDiag(X, 1, 1.0);
    setDiag(X, -1, -1.0);

    var M: [X.domain] complex;

    for 1..iters {
      t.start();
      M = expm(X);
      t.stop();
    }

    if !correctness then
      writeln('LinearAlgebra.expm_complex: ' + ((t.elapsed()/iters): string));
    else
      writeln(M);
    writeln("");

    t.clear();
  }
}

proc test_sinm() {
    // Testing eltType=Complex: Input Matrix of Format
    // [ 1.0 + 1i 1.0       0.0     ]
    // [-1.0      1.0 + 1i  1.0     ]
    // [ 0.0     -1.0       1.0 + 1i]

    const Space = {0..<m, 0..<m};
    var t: Timer;

    var X: [Space] complex;
    setDiag(X, 0, 1.0 + 1i);
    setDiag(X, 1, 1.0);
    setDiag(X, -1, -1.0);

    var M: [X.domain] complex;

    for 1..iters {
      t.start();
      M = sinm(X);
      t.stop();
    }

    if !correctness then
      writeln('LinearAlgebra.sinm: ' + ((t.elapsed()/iters): string));
    else
      writeln(M);
    writeln("");

    t.clear();
}

proc test_cosm() {
    // Testing eltType=Complex: Input Matrix of Format
    // [ 1.0 + 1i 1.0       0.0     ]
    // [-1.0      1.0 + 1i  1.0     ]
    // [ 0.0     -1.0       1.0 + 1i]

    const Space = {0..<m, 0..<m};
    var t: Timer;

    var X: [Space] complex;
    setDiag(X, 0, 1.0 + 1i);
    setDiag(X, 1, 1.0);
    setDiag(X, -1, -1.0);

    var M: [X.domain] complex;

    for 1..iters {
      t.start();
      M = cosm(X);
      t.stop();
    }

    if !correctness then
      writeln('LinearAlgebra.cosm: ' + ((t.elapsed()/iters): string));
    else
      writeln(M);
    writeln("");

    t.clear();
}

proc test_sincos() {
    // Testing eltType=Complex: Input Matrix of Format
    // [ 1.0 + 1i 1.0       0.0     ]
    // [-1.0      1.0 + 1i  1.0     ]
    // [ 0.0     -1.0       1.0 + 1i]

    const Space = {0..<m, 0..<m};
    var t: Timer;

    var X: [Space] complex;
    setDiag(X, 0, 1.0 + 1i);
    setDiag(X, 1, 1.0);
    setDiag(X, -1, -1.0);

    // var Z_sin: [X.domain] complex;
    // var Z_cos: [X.domain] complex;
    // var M;

    for 1..iters {
      t.start();
      sincos(X);
      t.stop();
    }

    if !correctness then
      writeln('LinearAlgebra.sincos: ' + ((t.elapsed()/iters): string));
    else
      writeln("");
    writeln("");

    t.clear();
}

proc main() {

  // Small Matrices
  m = 4;
  test_expm();
  test_sinm();
  test_cosm();
  test_sincos();

  // Medium Matrices
  m = 20;
  test_expm();
  test_sinm();
  test_cosm();
  test_sincos();

  // // Large Matrices
  // m = 100;
  // test_expm();
  // test_sinm();
  // test_cosm();
  // test_sincos();
}
