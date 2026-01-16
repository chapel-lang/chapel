use UnitTest;

proc testAssert(actual, expected, absTol, relTol, equalNan=true, reason="") {
  var sep = "=="*40;
  var test = new Test();
  try {
    test.assertClose(actual=actual, expected=expected, absTol=absTol,
                     relTol=relTol, equalNan=equalNan);
  } catch e {
    writeln("Error Caught in "+reason);
    writeln(e);
    writeln(sep);
  }
}


proc main() {
  negativeRelTol();
  negativeAbsTol();
  equalArray();
  unequalComplexArray();
  mismatchedArraySize();
  equalReal();
  unequalComplex();
  unequalNanRealImag();
  unequalNanComplex();
  equalNanReal();
  equalNanImag();
  equalNanComplex();
}

proc negativeRelTol() {
  testAssert(1.5, 1.6, -.1, 1e-15, reason="Negative relative tolerance");
}

proc negativeAbsTol() {
  testAssert(1.5, 1.6, .1, -1e-15, reason="Negative absolute tolerance");
}

proc equalArray() {
  var a: [0..4] real = [.3, .31, .35, -.1, -1e-9];
  var e: [1..5] real = [.3, .32, .35, -.1, -1e-9];
  testAssert(a, e, 0.3, 0.0, reason="Equal 1D arrays");
}

proc unequalComplexArray() {
  var a = [.3+1i, 2-3i];
  var e = [.3+1i, 3-2i];
  testAssert(a, e, 1.0, 0.0, reason="Unequal complex 1D arrays");
}

proc mismatchedArraySize() {
  var a = [.3, .31, .35, -.1, -1e-9];
  var e = [.3, .32, .35, -.1];
  testAssert(a, e, 0.0, 0.0, reason="Mismatched array sizes");
}

proc equalReal() {
  testAssert(1.8, 2.0, 0.0, 0.1, reason="Equal real numbers");
}

proc unequalComplex() {
  testAssert(1.8+3i, 1.8+3.000001i, 0.0, 1e-15,
             reason="Unequal complex numbers");
}

proc unequalNanRealImag() {
  testAssert(nan, 2.5, 0.0, 1e-15, reason="Unequal real NaN");
  testAssert(nan, 2.5, 0.0, 1e-15, equalNan=false, reason="Unequal real NaN");
  testAssert([3.1, nan, 6.5], [4.1, nan, 6.5], 1.0, 0.0, equalNan=false,
             reason="Unequal real array NaN");
  testAssert(-1.0, nan, 0.0, 1e-15, reason="Unequal real NaN");
  testAssert(0.0, nan, 0.0, 1e-15, equalNan=false, reason="Unequal real NaN");
  testAssert(nan*1i, nan*1i, 0.0, 1e-15, equalNan=false,
             reason="Unequal imag NaN");
  testAssert([3.1i, 14i, 6.5i], [4.1i, nan*1i, 6.5i], 1.0, 0.0,
             reason="Unequal imag array NaN");
}
proc unequalNanComplex() {
  testAssert(nan+2.0i, 2.5+2.0i, 0.0, 1e-15, reason="Unequal complex NaN");
  testAssert(1.0+nan*1i, 1.0+2.5i, 0.0, 1e-15, reason="Unequal complex NaN");
  testAssert(1.0+nan*1i, 1.0+nan*1i, 0.0, 1e-15, reason="Unequal complex NaN");
  testAssert(nan+nan*1i, nan+nan*1i, 0.0, 1e-15, equalNan=false,
             reason="Unequal complex NaN");
}
proc equalNanReal() {
  testAssert(nan, nan, 3.0, 5.0, reason="Equal real NaN");
  testAssert([3.1, nan, 6.5], [4.1, nan, 6.5], 1.0, 0.0,
             reason="Equal real NaN");
}
proc equalNanImag() {
  testAssert(nan*1i, nan*1i, 0.0, 1e-15, reason="Equal imag NaN");
  testAssert([nan*1i, 3.5i], [nan*1i, 3.9i], 0.5, 0.0,
             reason="Equal imag array NaN");
}
proc equalNanComplex() {
  testAssert(nan+nan*1i, nan+nan*1i, 0.0, 1e-15, reason="Equal complex NaN");
  testAssert([nan+nan*1i, 14+9i], [nan+nan*1i, 16+9i], 2.0, 0.0,
             reason="Equal complex array NaN");
}