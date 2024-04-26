use Math;
// commented out versions that have been deprecated and changed
{
  var x: imag(32) = 1i;
  // asin(x);
  asinh(x);
  atan(x);
  // atanh(x);
  // cos(x);
  // cosh(x);
  // sin(x);
  // sinh(x);
  // tan(x);
  // tanh(x);
}
{
  var x: imag(64) = 1i;
  // asin(x);
  asinh(x);
  atan(x);
  // atanh(x);
  // cos(x);
  // cosh(x);
  // sin(x);
  // sinh(x);
  // tan(x);
  // tanh(x);
}
{
  // this case should not warn
  var x: complex = 1i;
  // asin(x);
  asinh(x);
  atan(x);
  // atanh(x);
  // cos(x);
  // cosh(x);
  // sin(x);
  // sinh(x);
  // tan(x);
  // tanh(x);
}
