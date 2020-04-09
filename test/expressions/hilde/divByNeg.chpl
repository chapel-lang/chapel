// divByNeg.chpl
//
// Test that division by a negative number is well-defined.
//

// Move through the four quadrants.
var a = 11;
var b = 3;
writeDivMod(a, b);

a = -11;
writeDivMod(a, b);

b = -3;
writeDivMod(a, b);

a = 11;
writeDivMod(a, b);

proc writeDivMod(n, d)
{
  // n - dividend
  // d - divisor
  // q - quotient
  // r - remainder
  var q = n / d;
  var r = n % d;

  // The identity q * d + r = n should always be true.
  if q * d + r != n then
    writeln("Integer division identity does not hold.");

  // We want the remainder to obey two additional properties:
  // 1. Its sign is the same as that of the dividend.
  // 2. Its absolute value is less than that of the divisor.
  // This guarantees that the quotient is as large as it can be in absolute value
  // but q * d is still closer to zero than n.
  if sgn(r) != sgn(n) then
    writeln("The sign of the remainder does not match the sign of the dividend.");
  if abs(r) >= abs(d) then
    writeln("The remainder is not less than the divisor.");

  // Write out the results, so they can be matched.
  writeln(n, " div ", d, " = ", q);
  writeln(n, " mod ", d, " = ", r);
}

