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
  equalReal();
}

proc equalArray() {
  var a: [1..5] real = [.3, .31, .35, -.1, -1e-9];
  var e: [1..5] real = [.3, .32, .35, -.1, -1e-9];
  testAssert(a, e, 0.3, 0.0, "Equal 1D Arrays");
}

proc equalReal() {
  testAssert(1.8, 2.0, 0.0, 0.1, "Equal real numbers");
}