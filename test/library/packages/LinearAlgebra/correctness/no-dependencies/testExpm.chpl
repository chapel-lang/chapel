use LinearAlgebra;
use LinearAlgebra.Sparse;
use LayoutCS;
use TestUtils;

{
  // Input Matrix
  // [1.0 1.0]
  // [0.0 1.0]

  const vec = 0..1;
  var D = {vec,vec};
  var X = eye(D);
  var Y = eye(D);
  X(0,1) = 1;
  X = 2*X;

  const AparentDom = {vec,vec};
  var ADom: sparse subdomain(AparentDom) dmapped CS(compressRows=true, sortedIndices=true);
  var A: [ADom] real;
  ADom += [(0,0),(0,1),(1,1)];

  Y(0,0) = 7.389056;
  Y(0,1) = 14.778112;
  Y(1,0) = 0.0;
  Y(1,1) = 7.389056;

  // Test 2*2 Matrix using norm.
  var E = expm(X);
  assertAlmostEqual(E, Y, 'Matrix Exponential for 2*2 Dense using norm - X = 2*2');

  // Test 2*2 Matrix using onenormest function.
  E = expm(X, false);
  assertAlmostEqual(E, Y, 'Matrix Exponential for 2*2 Dense using onenormest - X = 2*2');

  // Test for 2*2 Sparse Matrix using onenormest function.
  A(0,0) = 2;
  A(0,1) = 2;
  A(1,1) = 2;
  E = expm(A, false);
  assertAlmostEqual(E, Y, 'Matrix Exponential for 2*2 Sparse using onenormest - X = 2*2');
}

{
  // Input Matrix
  // [1.0 1.0 1.0]
  // [0.0 1.0 0.0]
  // [1.0 0.0 1.0]

  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var Y = eye(D);
  X(0,1) = 1;
  X(0,2) = 1;
  X(2,0) = 1;
  X = 2*X;

  const AparentDom = {vec,vec};
  var ADom: sparse subdomain(AparentDom) dmapped CS(compressRows=true, sortedIndices=true);
  var A: [ADom] real;
  ADom += [(0,0),(1,1),(2,2),(0,1),(0,2),(2,0)];

  Y(0,0) = 27.799075;
  Y(0,1) = 26.799075;
  Y(0,2) = 26.799075;
  Y(1,0) = 0.0;
  Y(1,1) = 7.389056;
  Y(1,2) = 0.0;
  Y(2,0) = 26.799075;
  Y(2,1) = 20.410019;
  Y(2,2) = 27.799075;

  // Test 3*3 Matrix using norm.
  var E = expm(X);
  assertAlmostEqual(E, Y, 'Matrix Exponential for 3*3 Dense Matrix using norm - X = 3*3');

  // Test 3*3 Matrix using onenormest function.
  E = expm(X, false);
  assertAlmostEqual(E, Y, 'Matrix Exponential for 3*3 Dense Matrix using onenormest - X = 3*3');

  // Test for 3*3 Sparse Matrix using onenormest function.
  A(0,1) = 2;
  A(0,2) = 2;
  A(2,0) = 2;
  for j in vec do A[j,j] = 2;
  E = expm(A, false);
  assertAlmostEqual(E, Y, 'Matrix Exponential for 3*3 Sparse Matrix using onenormest - X = 3*3');
}

{
  //Pade3, Pade5, Pade7, Pade9, Pade13 cases
  // Input Matrices of Format
  // [ x  0.0 0.0]
  // [0.0  x  0.0]
  // [0.0 0.0  x ]
  // were x = [0.01, 0.1, 0.5, 1.0, 10.0]
  const vec = 0..2;
  var D = {vec,vec};
  const AparentDom = {vec,vec};
  var ADom: sparse subdomain(AparentDom) dmapped CS(compressRows=true, sortedIndices=true);
  ADom += [(0,0),(1,1),(2,2)];

  const b = [0.01, 0.1, 0.5, 1.0, 10.0];
  const res = [1.010050, 1.105171, 1.648721, 2.718282, 22026.465795];
  const pade = [3,5,7,9,13];

  for i in 0..4 do {
    var X = eye(D);
    var Y = X * b[i];
    var A: [ADom] real;
    setDiag(X, 0, res[i]);
    for j in vec do A[j,j] = b[i];

    // Test 3*3 Dense Matrix Pade cases using norm.
    var E = expm(Y);
    assertAlmostEqual(E, X, 'Matrix Exponential for 3*3 Dense Matrix using norm - X = 3*3 : Uses Pade' + (pade[i]: string));

    // Test 3*3 Dense Matrix Pade cases using onenormest function.
    E = expm(Y, false);
    assertAlmostEqual(E, X, 'Matrix Exponential for 3*3 Dense Matrix using onenormest - X = 3*3 : Uses Pade' + (pade[i]: string));

    // Test 3*3 Sparse Matrix Pade cases using onenormest function.
    E = expm(A, false);
    assertAlmostEqual(E, X, 'Matrix Exponential for 3*3 Sparse Matrix using onenormest - X = 3*3 : Uses Pade' + (pade[i]: string));
  }
}

{
  // Input Matrices of Format
  // [1.0 10^n 0.0]
  // [0.0 -1.0 0.0]
  // [0.0  0.0 1.0]
  // where n = [0,1,2,3,4,5,6,7,8]
  const vec = 0..2;
  var D = {vec,vec};
  var ADom: sparse subdomain(D) dmapped CS(compressRows=true, sortedIndices=true);
  ADom += [(0,0),(1,1),(2,2),(0,1)];

  const b = [1.0, 10.0, 100.0, 1000.0, 10000.0, 100000.0, 1000000.0, 10000000.0, 100000000.0];
  const res = [1.175201, 11.752012, 117.520119, 1175.201194, 11752.011936, 117520.119364, 1175201.193644, 11752011.936438, 117520119.364380];

  var X = eye(D);
  X(0,0) = 1.0;
  X(1,0) = 0.0;
  X(1,1) = -1.0;

  var Y = eye(D);
  setDiag(Y, 0, 2.718282);
  Y(1,1) = 0.367879;

  var A: [ADom] real;
  A(0,0) = 1.0;
  A(1,1) = -1.0;
  A(2,2) = 1.0;

  for i in 0..8 do {
    X(0,1) = b[i];
    A(0,1) = b[i];
    Y(0,1) = res[i];

    // Test expm for Dense Matrix using norm function.
    var E = expm(X);
    assertAlmostEqual(E, Y, ' ');

    // Test expm for Dense Matrix using onenormest function.
    E = expm(X, false);
    assertAlmostEqual(E, Y, ' ');

    // Test expm for Sparse Matrix using onenormest function.
    E = expm(A, false);
    assertAlmostEqual(E, Y, ' ');
  }
}

{
  // Input Matrix
  // [ 0.0  1.0  0.0]
  // [-1.0  0.0  1.0]
  // [ 0.0 -1.0  0.0]
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var Y = eye(D);
  var ADom: sparse subdomain(D) dmapped CS(compressRows=true, sortedIndices=true);
  ADom += [(1,0),(0,1),(2,1),(1,2)];

  setDiag(X, 0, 0.0);
  setDiag(X, 1, 1.0);
  setDiag(X, -1, -1.0);

  var A: [ADom] real;
  A(1,0) = -1.0;
  A(0,1) = 1.0;
  A(2,1) = -1.0;
  A(1,2) = 1.0;

  Y(0,0) = 0.577972;
  Y(0,1) = 0.698456;
  Y(0,2) = 0.422028;
  Y(1,0) = -0.698456;
  Y(1,1) = 0.155944;
  Y(1,2) = 0.698456;
  Y(2,0) = 0.422028;
  Y(2,1) = -0.698456;
  Y(2,2) = 0.577972;

  // Test expm for Dense Matrix using norm function.
  var E = expm(X);
  assertAlmostEqual(E, Y, 'Check Gradient Dense Matrix using norm function');

  // Test expm for Dense Matrix using onenormest function.
  E = expm(X, false);
  assertAlmostEqual(E, Y, 'Check Gradient Dense Matrix using onenormest function');

  // Test expm for Sparse Matrix using onenormest function.
  E = expm(A, false);
  assertAlmostEqual(E, Y, 'Check Gradient Sparse Matrix using onenormest function');
}

{
  // Input Matrix
  // [-2.0  1.0  0.0]
  // [ 1.0 -2.0  1.0]
  // [ 0.0  1.0 -2.0]
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var Y = eye(D);
  var ADom: sparse subdomain(D) dmapped CS(compressRows=true, sortedIndices=true);
  ADom += [(0,0),(1,1),(2,2),(1,0),(0,1),(2,1),(1,2)];

  setDiag(X, 0, -2.0);
  setDiag(X, 1, 1.0);
  setDiag(X, -1, 1.0);

  var A: [ADom] real;
  A(1,0) = 1.0;
  A(0,1) = 1.0;
  A(2,1) = 1.0;
  A(1,2) = 1.0;
  for j in vec do A[j,j] = -2.0;

  Y(0,0) = 0.215060;
  Y(0,1) = 0.185179;
  Y(0,2) = 0.079725;
  Y(1,0) = 0.185179;
  Y(1,1) = 0.294785;
  Y(1,2) = 0.185179;
  Y(2,0) = 0.079725;
  Y(2,1) = 0.185179;
  Y(2,2) = 0.215060;

  // Test expm for Dense Matrix using norm function.
  var E = expm(X);
  assertAlmostEqual(E, Y, 'Check Edge Matrix using norm function');

  // Test expm for Dense Matrix using onenormest function.
  E = expm(X, false);
  assertAlmostEqual(E, Y, 'Check Edge Matrix using onenormest function');

  // Test expm for Sparse Matrix using onenormest function.
  E = expm(A, false);
  assertAlmostEqual(E, Y, 'Check Edge Sparse Matrix using onenormest function');
}

{
  // Test for sinm, cosm, expm
  // Input Matrix
  // [1.0 + 1i  1.0 + 0i  0.0 + 0i]
  // [1.0 + 0i  1.0 + 1i  1.0 + 0i]
  // [0.0 + 0i  1.0 + 0i  1.0 + 1i]

  const vec = 0..2;
  var D = {vec,vec};
  var X:[D] complex;
  var ADom: sparse subdomain(D) dmapped CS(compressRows=true, sortedIndices=true);
  ADom += [(0,0),(1,1),(2,2),(1,0),(0,1),(2,1),(1,2)];

  setDiag(X, 0, 1.0 + 1i);
  setDiag(X, 1, 1.0);
  setDiag(X, -1, 1.0);

  var A: [ADom] complex;
  A(1,0) = 1.0;
  A(0,1) = 1.0;
  A(2,1) = 1.0;
  A(1,2) = 1.0;
  for j in vec do A[j,j] = 1.0 + 1i;
  var iA = 1.0i * A;

  var Z_cos = cosm(X);
  var Z_sin = sinm(X);
  var exp_iX = Z_cos + 1.0i * Z_sin;
  var iX = 1.0i * X;

  // Test sinm, cosm, expm for Dense Matrix using norm function.
  var E = expm(iX);
  assertAlmostEqual(E, exp_iX, 'Check sinm, cosm, expm for Dense Matrix using norm function');

  // Test sinm, cosm, expm for Dense Matrix using onenormest function.
  E = expm(iX, false);
  assertAlmostEqual(E, exp_iX, 'Check sinm, cosm, expm for Dense Matrix using onenormest function');

  // Test sinm, cosm, expm for Sparse Matrix using onenormest function.
  E = expm(iA, false);
  assertAlmostEqual(E, exp_iX, 'Check sinm, cosm, expm for Sparse Matrix using onenormest function');
}

{
  // Test for sincos, expm
  // Input Matrix
  // [1.0 + 1i  1.0 + 0i  0.0 + 0i]
  // [1.0 + 0i  1.0 + 1i  1.0 + 0i]
  // [0.0 + 0i  1.0 + 0i  1.0 + 1i]

  const vec = 0..2;
  var D = {vec,vec};
  var X:[D] complex;
  var ADom: sparse subdomain(D) dmapped CS(compressRows=true, sortedIndices=true);
  ADom += [(0,0),(1,1),(2,2),(1,0),(0,1),(2,1),(1,2)];

  setDiag(X, 0, 1.0 + 1i);
  setDiag(X, 1, 1.0);
  setDiag(X, -1, 1.0);

  var A: [ADom] complex;
  A(1,0) = 1.0;
  A(0,1) = 1.0;
  A(2,1) = 1.0;
  A(1,2) = 1.0;
  for j in vec do A[j,j] = 1.0 + 1i;
  var iA = 1.0i * A;

  var (Z_sin, Z_cos) = sincos(X);
  var exp_iX = Z_cos + 1.0i * Z_sin;
  var iX = 1.0i * X;

  // Test sincos, expm for Dense Matrix using norm function.
  var E = expm(iX);
  assertAlmostEqual(E, exp_iX, 'Check sincos, expm for Dense Matrix using norm function');

  // Test sincos, expm for Dense Matrix using onenormest function.
  E = expm(iX, false);
  assertAlmostEqual(E, exp_iX, 'Check sincos, expm for Dense Matrix using onenormest function');

  // Test sincos, expm for Sparse Matrix using onenormest function.
  E = expm(iA, false);
  assertAlmostEqual(E, exp_iX, 'Check sincos, expm for Sparse Matrix using onenormest function');
}
