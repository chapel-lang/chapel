//
// This test validates that the trigonometric and hyperbolic functions that
// take an imaginary argument return the same results as the corresponding
// functions that take a complex argument.
//

use Math;

config const verbose = false;

iter testData(min, max, step) {
  var x = min;
  while x <= max {
    yield x;
    x += step;
  }
}

proc equal(a, b, tol=1e-5) {

  const diff = abs(a-b);
  if !isNan(diff) then return isClose(a, b, absTol=tol);

  if isImag(a) && isImag(b) then
    if isInf(a:real) && isInf(b:real) then return true;

  // if both complex, compare real and imaginary parts
  if isComplex(a) && isComplex(b) then
    return equal(a:real, b:real, tol) && equal(a:imag, b:imag, tol);
  // if one is complex and the other is imag, compare imag part and ensure real is 0
  else if isComplex(a) && isImag(b) then
    return equal(a:imag, b, tol) && equal(a:real, 0.0, tol);
  else if isComplex(b) && isImag(a) then
    return equal(a, b:imag, tol) && equal(b:real, 0.0, tol);
  // if one is complex and the other is real, compare real part and ensure imag is 0
  else if isComplex(a) && isReal(b) then
    return equal(a:real, b, tol) && equal(a:imag, 0.0i, tol);
  else if isComplex(b) && isReal(a) then
    return equal(a, b:real, tol) && equal(b:imag, 0.0i, tol);

  // if we reach this point, comparison is not implemented
  halt("Cannot compare " + a:string + " (" + a.type:string + ") and " + b:string + " (" + b.type:string + ")");
}

proc check(func: string, x, a, b) {
  if verbose {
    writeln("Checking ", func, "(", x, "): ", a, " == ", b);
  }
  assert(equal(a, b));
}

proc test(type imagType, type complexType) {
  for data in testData(-20.0, 20.0, 0.25) {
    var x = data:imagType;
    check("asin",  x, asin(x),  asin(x:complexType));
    check("asinh", x, asinh(x), asinh(x:complexType));
    check("atan",  x, atan(x),  atan(x:complexType));
    check("atanh", x, atanh(x), atanh(x:complexType));
    check("cos",   x, cos(x),   cos(x:complexType));
    check("cosh",  x, cosh(x),  cosh(x:complexType));
    check("sin",   x, sin(x),   sin(x:complexType));
    check("sinh",  x, sinh(x),  sinh(x:complexType));
    check("tan",   x, tan(x),   tan(x:complexType));
    check("tanh",  x, tanh(x),  tanh(x:complexType));
  }
}

test(imag(32), complex);
test(imag(64), complex);
