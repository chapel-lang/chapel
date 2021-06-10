use LinearAlgebra;
use TestUtils;

{
  const vec = 0..1;
  var D = {vec,vec};
  var X = eye(D);
  X(0,1) = 1;
  X = reshape(2*X, D);

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
  X = reshape(2*X, D);

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
  //Pade3 case
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var w = 0.01;
  var Y = X * w;

  var E = expm(Y);

  X(0,0) = 1.010050;
  X(0,1) = 0.0;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 1.010050;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 1.010050;

  assertAlmostEqual(E, X, 'Matrix Exponential - X = 3*3 (Uses Pade3)');
}

{
  //Pade5 case
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var w = 0.1;
  var Y = X * w;

  var E = expm(Y);

  X(0,0) = 1.105171;
  X(0,1) = 0.0;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 1.105171;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 1.105171;

  assertAlmostEqual(E, X, 'Matrix Exponential - X = 3*3 (Uses Pade5)');
}

{
  //Pade7 case
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var w = 0.5;
  var Y = X * w;

  var E = expm(Y);

  X(0,0) = 1.648721;
  X(0,1) = 0.0;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 1.648721;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 1.648721;

  assertAlmostEqual(E, X, 'Matrix Exponential - X = 3*3 (Uses Pade7)');
}

{
  //Pade9 case
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var w = 1;
  var Y = X * w;

  var E = expm(Y);

  X(0,0) = 2.718282;
  X(0,1) = 0.0;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 2.718282;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 2.718282;

  assertAlmostEqual(E, X, 'Matrix Exponential - X = 3*3 (Uses Pade9)');
}

{
  //Pade13 case
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var w = 10;
  var Y = X * w;

  var E = expm(Y);

  X(0,0) = 22026.465795;
  X(0,1) = 0.0;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 22026.465795;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 22026.465795;

  assertAlmostEqual(E, X, 'Matrix Exponential - X = 3*3 (Uses Pade13)');
}

{
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var b = 1.0;
  X(0,0) = 1.0;
  X(0,1) = b;
  X(1,0) = 0.0;
  X(1,1) = -1.0;

  var E = expm(X);

  X(0,0) = 2.718282;
  X(0,1) = 1.175201;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 0.367879;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 2.718282;

  assertAlmostEqual(E, X, ' ');
}

{
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var b = 10.0;
  X(0,0) = 1.0;
  X(0,1) = b;
  X(1,0) = 0.0;
  X(1,1) = -1.0;

  var E = expm(X);

  X(0,0) = 2.718282;
  X(0,1) = 11.752012;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 0.367879;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 2.718282;

  assertAlmostEqual(E, X, ' ');
}

{
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var b = 100.0;
  X(0,0) = 1.0;
  X(0,1) = b;
  X(1,0) = 0.0;
  X(1,1) = -1.0;

  var E = expm(X);

  X(0,0) = 2.718282;
  X(0,1) = 117.520119;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 0.367879;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 2.718282;

  assertAlmostEqual(E, X, ' ');
}

{
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var b = 1000.0;
  X(0,0) = 1.0;
  X(0,1) = b;
  X(1,0) = 0.0;
  X(1,1) = -1.0;

  var E = expm(X);

  X(0,0) = 2.718282;
  X(0,1) = 1175.201194;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 0.367879;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 2.718282;

  assertAlmostEqual(E, X, ' ');
}

{
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var b = 10000.0;
  X(0,0) = 1.0;
  X(0,1) = b;
  X(1,0) = 0.0;
  X(1,1) = -1.0;

  var E = expm(X);

  X(0,0) = 2.718282;
  X(0,1) = 11752.011936;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 0.367879;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 2.718282;

  assertAlmostEqual(E, X, ' ');
}

{
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var b = 100000.0;
  X(0,0) = 1.0;
  X(0,1) = b;
  X(1,0) = 0.0;
  X(1,1) = -1.0;

  var E = expm(X);

  X(0,0) = 2.718282;
  X(0,1) = 117520.119364;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 0.367879;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 2.718282;

  assertAlmostEqual(E, X, ' ');
}

{
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var b = 1000000.0;
  X(0,0) = 1.0;
  X(0,1) = b;
  X(1,0) = 0.0;
  X(1,1) = -1.0;

  var E = expm(X);

  X(0,0) = 2.718282;
  X(0,1) = 1175201.193644;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 0.367879;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 2.718282;

  assertAlmostEqual(E, X, ' ');
}

{
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var b = 10000000.0;
  X(0,0) = 1.0;
  X(0,1) = b;
  X(1,0) = 0.0;
  X(1,1) = -1.0;

  var E = expm(X);

  X(0,0) = 2.718282;
  X(0,1) = 11752011.936438;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 0.367879;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 2.718282;

  assertAlmostEqual(E, X, ' ');
}

{
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var b = 100000000.0;
  X(0,0) = 1.0;
  X(0,1) = b;
  X(1,0) = 0.0;
  X(1,1) = -1.0;

  var E = expm(X);

  X(0,0) = 2.718282;
  X(0,1) = 117520119.364380;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 0.367879;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 2.718282;

  assertAlmostEqual(E, X, ' ');
}

{
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  X(0,0) = 0.0;
  X(1,1) = 0.0;
  X(2,2) = 0.0;
  X(0,1) = 1.0;
  X(1,0) = -1.0;
  X(1,2) = 1.0;
  X(2,1) = -1.0;

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
  X(0,0) = -2.0;
  X(1,1) = -2.0;
  X(2,2) = -2.0;
  X(0,1) = 1.0;
  X(1,0) = 1.0;
  X(1,2) = 1.0;
  X(2,1) = 1.0;

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
