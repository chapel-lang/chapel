use UnitTest;

proc testAssert(actual, expected, absTol=0.0, relTol=1e-15,
                equalNan=true, reason="") {
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
  equalReal();
  equalRealArray();
  equalImag();
  equalComplex();
  equalComplexArray();

  equalNanReal();
  equalNanImag();
  equalNanComplex();

  negativeRelTol();
  negativeAbsTol();
  mismatchedArraySize();

  unequalReal();
  unequalComplex();
  unequalComplexArray();

  unequalNanReal();
  unequalNanImag();
  unequalNanComplex();
}

proc equalReal() {
  testAssert(1.8, 2.0, relTol=0.1, reason="Equal real numbers");
}

proc equalRealArray() {
  var a: [0..4] real = [.3, .31, .35, -.1, -1e-9];
  var e: [1..5] real = [.3, .32, .6, -.2, -2e-9];
  testAssert(a, e, absTol=0.3, relTol=0.0, reason="Equal real 1D arrays");
}

proc equalImag() {
  testAssert(1.8i, 2.0i, relTol=0.1, reason="Equal imag numbers");
}
proc equalComplex() {
  testAssert(-13+2.5i, -13+2.4i, relTol=8e-3, reason="Equal complex numbers");
}
proc equalComplexArray() {
  var a: [0..2, 0..1] complex = [2+0.99i, 1+1i; -3+1i, 5+1i; 6+1i, 7+1i];
  var e: [1..3, 0..1] complex = [2+1i, 1+1i; -2.99+1i, 5+1i; 6+1i, 7+1i];
  testAssert(a, e, relTol=0.01, reason="Equal complex 2D arrays");
}

proc equalNanReal() {
  testAssert(nan, nan, reason="Equal real NaN");
  testAssert([3.1, nan, 6.5], [4.1, nan, 6.5], absTol=1.0, relTol=0.0,
             reason="Equal real NaN array");
}
proc equalNanImag() {
  testAssert(nan*1i, nan*1i, reason="Equal imag NaN");
  testAssert([nan*1i, 3.5i], [nan*1i, 3.9i], absTol=0.5, relTol=0.0,
             reason="Equal imag NaN array");
}
proc equalNanComplex() {
  testAssert(nan+nan*1i, nan+nan*1i, reason="Equal complex NaN");
  testAssert(1.0+nan*1i, 1.0+nan*1i, reason="Equal complex partial NaN array");
  testAssert([nan+nan*1i, 14+9i], [nan+nan*1i, 16+9i],
             absTol=2.0, relTol=0.0, reason="Equal complex NaN array");
}

proc negativeRelTol() {
  testAssert(1.5, 1.6, absTol=-0.1, reason="Negative relative tolerance");
}

proc negativeAbsTol() {
  testAssert(1.5, 1.6, absTol=0.1, relTol=-1e-15,
             reason="Negative absolute tolerance");
}

proc mismatchedArraySize() {
  var a = [.3, .31, .35, -.1, -1e-9];
  var e = [.3, .32, .35, -.1];
  testAssert(a, e, reason="Mismatched array sizes");
}

proc unequalReal() {
  testAssert(0.0, 1e-6, absTol=9.9e-7, reason="Unequal real numbers");
}
proc unequalComplex() {
  testAssert(1.8+3i, 1.8+3.000001i, reason="Unequal complex numbers");
}

proc unequalComplexArray() {
  var a = [.3+1i, 2-3i];
  var e = [.3+1i, 3-2i];
  testAssert(a, e, absTol=1.0, relTol=0.0, reason="Unequal complex 1D arrays");
}

proc unequalNanReal() {
  testAssert(nan, 2.5, reason="Unequal real NaN, equalNan true");
  testAssert(nan, 2.5, equalNan=false,
             reason="Unequal real NaN, equalNan false");
  testAssert([3.1, nan, 6.5], [3.1, nan, 6.5], equalNan=false,
             reason="Unequal real array NaN");
}

proc unequalNanImag() {
  testAssert(nan*1i, nan*1i, equalNan=false,
             reason="Unequal imag NaN");
  testAssert([3.1i, 14i, 6.5i], [4.1i, nan*1i, 6.5i],
             absTol=1.0, relTol=0.0, reason="Unequal imag array NaN");
}

proc unequalNanComplex() {
  testAssert(nan+2.0i, 2.5+2.0i, reason="Unequal complex partial NaN, real");
  testAssert(1.0+nan*1i, 1.0+2.5i, reason="Unequal complex partial NaN, imag");
  testAssert(nan+nan*1i, nan+nan*1i, equalNan=false,
             reason="Unequal complex NaN");
}
