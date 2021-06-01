use LinearAlgebra;
use TestUtils;

{
  const vec = 0..1;
  var D = {vec,vec};
  var X = eye(D);
  X(0,1) = 1;
  X = reshape(2*X, D);

  var E = expm(X);

  X(0,0) = 7.38906;
  X(0,1) = 14.7781;
  X(1,0) = 0.0;
  X(1,1) = 7.38906;

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

  X(0,0) = 27.7991;
  X(0,1) = 26.7991;
  X(0,2) = 26.7991;
  X(1,0) = 0.0;
  X(1,1) = 7.38906;
  X(1,2) = 0.0;
  X(2,0) = 26.7991;
  X(2,1) = 20.41;
  X(2,2) = 27.7991;

  assertAlmostEqual(E, X, 'Matrix Exponential - X = 3*3');
}

{
  //Pade3 case
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var w = 0.01;
  Y = X * w;

  var E = expm(Y);

  X(0,0) = 1.01005;
  X(0,1) = 0.0;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 1.01005;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 1.01005;

  assertAlmostEqual(E, X, 'Matrix Exponential - X = 3*3 (Uses Pade3)');
}

{
  //Pade5 case
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var w = 0.1;
  Y = X * w;

  var E = expm(Y);

  X(0,0) = 1.10517;
  X(0,1) = 0.0;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 1.10517;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 1.10517;

  assertAlmostEqual(E, X, 'Matrix Exponential - X = 3*3 (Uses Pade5)');
}

{
  //Pade7 case
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var w = 0.5;
  Y = X * w;

  var E = expm(Y);

  X(0,0) = 1.64872;
  X(0,1) = 0.0;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 1.64872;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 1.64872;

  assertAlmostEqual(E, X, 'Matrix Exponential - X = 3*3 (Uses Pade7)');
}

{
  //Pade9 case
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var w = 1;
  Y = X * w;

  var E = expm(Y);

  X(0,0) = 2.71828;
  X(0,1) = 0.0;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 2.71828;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 2.71828;

  assertAlmostEqual(E, X, 'Matrix Exponential - X = 3*3 (Uses Pade9)');
}

{
  //Pade13 case
  const vec = 0..2;
  var D = {vec,vec};
  var X = eye(D);
  var w = 10;
  Y = X * w;

  var E = expm(Y);

  X(0,0) = 22026.5;
  X(0,1) = 0.0;
  X(0,2) = 0.0;
  X(1,0) = 0.0;
  X(1,1) = 22026.5;
  X(1,2) = 0.0;
  X(2,0) = 0.0;
  X(2,1) = 0.0;
  X(2,2) = 22026.5;

  assertAlmostEqual(E, X, 'Matrix Exponential - X = 3*3 (Uses Pade13)');
}
