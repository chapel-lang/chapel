use UnitTest;

proc testAssert(actual, expected, absTol, relTol, reason="") {
  var sep = "=="*40;
  var test = new Test();
  try {
    test.assertClose(actual=actual, expected=expected, absTol=absTol,
                     relTol=relTol);
  } catch e {
    writeln("Error Caught in "+reason);
    writeln(e);
    writeln(sep);
  }
}


proc main() {
  equalArray();
  unequalComplexArray();
  mismatchedArraySize();
  equalReal();
  unequalComplex();
}

proc equalArray() {
  var a: [0..4] real = [.3, .31, .35, -.1, -1e-9];
  var e: [1..5] real = [.3, .32, .35, -.1, -1e-9];
  testAssert(a, e, 0.3, 0.0, "Equal 1D arrays");
}

proc unequalComplexArray() {
  var a = [.3+1i, 2-3i];
  var e = [.3+1i, 3-2i];
  testAssert(a, e, 1.0, 0.0, "Unequal complex 1D arrays");
}

proc mismatchedArraySize() {
  var a = [.3, .31, .35, -.1, -1e-9];
  var e = [.3, .32, .35, -.1];
  testAssert(a, e, 0.0, 0.0, "Mismatched array sizes");
}

proc equalReal() {
  testAssert(1.8, 2.0, 0.0, 0.1, "Equal real numbers");
}

proc unequalComplex() {
  testAssert(1.8+3i, 1.8+3.000001i, 0.0, 1e-15, "Unequal complex numbers");
}