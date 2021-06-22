use LinearAlgebra;
use TestUtils;

{
  const vec = 0..1;
  var D = {vec,vec};
  var X = eye(D);
  X(0,1) = 1;
  X = 2*X;

  var E = expm(X);

  X(0,0) = 7.389056;
  X(0,1) = 14.778112;
  X(1,0) = 0.0;
  X(1,1) = 7.389056;

  assertAlmostEqual(E, X, 'Matrix Exponential - X = 2*2');
}

{
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  X(0,1) = 1;
  X(0,2) = 1;
  X(2,0) = 1;
  X = 2*X;

  var E = expm(X);

  X(0,0) = 27.799075;
  X(0,1) = 26.799075;
  X(0,2) = 26.799075;
  X(1,0) = 0.0;
  X(1,1) = 7.389056;
  X(1,2) = 0.0;
  X(2,0) = 26.799075;
  X(2,1) = 20.410019;
  X(2,2) = 27.799075;

  assertAlmostEqual(E, X, 'Matrix Exponential - X = 3*3');
}

{
  //Pade3, Pade5, Pade7, Pade9, Pade13 cases
  const vec = 0..2;
  var D = {vec,vec};

  const b = [0.01, 0.1, 0.5, 1.0, 10.0];
  const res = [1.010050, 1.105171, 1.648721, 2.718282, 22026.465795];
  const pade = [3,5,7,9,13];

  for i in 0..4 do {
    var X = eye(D);
    var Y = X * b[i];
    var E = expm(Y);
    setDiag(X, 0, res[i]);

    assertAlmostEqual(E, X, 'Matrix Exponential - X = 3*3 : Uses Pade' + (pade[i]: string));
  }
}

{
  // Input Matrices of Format
  // [1.0 10^n 0.0]
  // [0.0 -1.0 0.0]
  // [0.0  0.0 1.0]
  const vec = 0..2;
  var D = {vec,vec};

  const b = [1.0, 10.0, 100.0, 1000.0, 10000.0, 100000.0, 1000000.0, 10000000.0, 100000000.0];
  const res = [1.175201, 11.752012, 117.520119, 1175.201194, 11752.011936, 117520.119364, 1175201.193644, 11752011.936438, 117520119.364380];

  var X = eye(D);
  X(0,0) = 1.0;
  X(1,0) = 0.0;
  X(1,1) = -1.0;

  var Y = eye(D);
  setDiag(Y, 0, 2.718282);
  Y(1,1) = 0.367879;

  for i in 0..8 do {
    X(0,1) = b[i];
    Y(0,1) = res[i];

    var E = expm(X);
    assertAlmostEqual(E, Y, ' ');
  }
}

{
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  setDiag(X, 0, 0.0);
  setDiag(X, 1, 1.0);
  setDiag(X, -1, -1.0);


  var E = expm(X);

  X(0,0) = 0.577972;
  X(0,1) = 0.698456;
  X(0,2) = 0.422028;
  X(1,0) = -0.698456;
  X(1,1) = 0.155944;
  X(1,2) = 0.698456;
  X(2,0) = 0.422028;
  X(2,1) = -0.698456;
  X(2,2) = 0.577972;

  assertAlmostEqual(E, X, ' ');
}

{
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  setDiag(X, 0, -2.0);
  setDiag(X, 1, 1.0);
  setDiag(X, -1, 1.0);

  var E = expm(X);

  X(0,0) = 0.215060;
  X(0,1) = 0.185179;
  X(0,2) = 0.079725;
  X(1,0) = 0.185179;
  X(1,1) = 0.294785;
  X(1,2) = 0.185179;
  X(2,0) = 0.079725;
  X(2,1) = 0.185179;
  X(2,2) = 0.215060;

  assertAlmostEqual(E, X, ' ');
}

{
  // Test for sinm, cosm, expm
  const vec = 0..2;
  var D = {vec,vec};
  var X:[D] complex;
  setDiag(X, 0, 1.0 + 1i);
  setDiag(X, 1, 1.0);
  setDiag(X, -1, 1.0);

  var Z_cos = cosm(X);
  var Z_sin = sinm(X);
  var exp_iX = Z_cos + 1.0i * Z_sin;
  var iX = 1.0i * X;

  var E = expm(iX);

  assertAlmostEqual(E, exp_iX, 'Check sinm, cosm, expm');
}

{
  // Test for sincos, expm
  const vec = 0..2;
  var D = {vec,vec};
  var X:[D] complex;
  setDiag(X, 0, 1.0 + 1i);
  setDiag(X, 1, 1.0);
  setDiag(X, -1, 1.0);

  var (Z_sin, Z_cos) = sincos(X);
  var exp_iX = Z_cos + 1.0i * Z_sin;
  var iX = 1.0i * X;

  var E = expm(iX);

  assertAlmostEqual(E, exp_iX, 'Check sincos, expm');
}
