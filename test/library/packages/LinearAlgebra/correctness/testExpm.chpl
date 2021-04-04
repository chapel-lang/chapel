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
