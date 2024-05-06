use Math;

{
  var x: imag(32) = 1i;
  asin(x);
  asinh(x); // should not warn
  atan(x); // should not warn
  atanh(x);
  cos(x);
  cosh(x);
  sin(x);
  sinh(x);
  tan(x);
  tanh(x);
}
{
  var x: imag(64) = 1i;
  asin(x);
  asinh(x); // should not warn
  atan(x); // should not warn
  atanh(x);
  cos(x);
  cosh(x);
  sin(x);
  sinh(x);
  tan(x);
  tanh(x);
}
{
  // this case should not warn
  var x: complex = 1i;
  asin(x);
  asinh(x); // should not warn
  atan(x); // should not warn
  atanh(x);
  cos(x);
  cosh(x);
  sin(x);
  sinh(x);
  tan(x);
  tanh(x);
}
