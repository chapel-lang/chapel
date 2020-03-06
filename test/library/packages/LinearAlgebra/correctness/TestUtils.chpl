/* Testing Utility Functions

  This module provides non-halting assertions with required error messages to
  help quickly identify test failures.

*/

config const correctness = true;

proc writeFailure(X, Y, msg, newline=true) {
  writeln("Test Failed: ", msg);
  if newline {
    writeln(X, '\n!=\n', Y);
  } else {
    writeln(X, ' != ', Y);
  }
}

proc assertEqual(X, Y, msg) {
  if !correctness then writeln(msg);
  if X != Y then writeFailure(X, Y, msg, newline=false);
}


proc assertEqual(X: [], Y: [], msg) {
  if !correctness then writeln(msg);
  if X.shape != Y.shape {
    writeFailure(X, Y, msg);
    return;
  } else if !X.equals(Y) {
    writeFailure(X, Y, msg);
    return;
  }
}

proc assertEqual(X: [], Y: [], msg) where isSparseArr(X) && isSparseArr(Y) {
  if !correctness then writeln(msg);
  if X.shape != Y.shape {
    writeFailure(X.shape, Y.shape, msg);
    return;
  } else if X.domain != Y.domain {
    writeFailure(X.domain, Y.domain, msg);
    return;
  } else if !X.equals(Y) {
    writeFailure(X, Y, msg);
    return;
  }
}

/* array.equals(array) overload for CS sparse arrays with different domains */
proc _array.equals(that: _array) where isSparseArr(that) && isSparseArr(this) {
  // First assert that domains share the same indices
  if this.domain != that.domain {
    return false;
  }
  // Then check that the values at each index are equal
  for (i,j) in this.domain {
    if this[i,j] != that[i,j] {
      return false;
    }
  }
  return true;
}


proc assertEqual(X: _tuple, Y: _tuple, msg) {
  if !correctness then writeln(msg);
  if X.size != Y.size {
    writeFailure(X, Y, msg, newline=false);
    return;
  } else {
    for (x, y) in zip(X, Y) {
      if x != y {
        writeFailure(X, Y, msg, newline=false);
        return;
      }
    }
  }
}

proc assertTrue(x: bool, msg) {
  if !correctness then writeln(msg);
  if !x {
    writeln('Test Failed: ', msg);
    writeln('Expected: true');
    writeln('Received: ', x);
  }
}

proc assertFalse(x: bool, msg) {
  if !correctness then writeln(msg);
  if x {
    writeln('Test Failed: ', msg);
    writeln('Expected: false');
    writeln('Received: ', x);
  }
}

proc assertAlmostEqual(X: [], Y: [], msg) {
  if !correctness then writeln(msg);
  if X.shape != Y.shape {
    writeFailure(X, Y, msg);
    return;
  } else if !almostEquals(X, Y) {
    writeFailure(X, Y, msg);
    return;
  }
}


/* TODO: contribute to standard library */
proc almostEquals(A: [], B: []): bool {
  //
  // quick path for identical arrays
  //
  if A._value == B._value then
    return true;
  //
  // quick path for rank mismatches
  //
  if A.rank != B.rank then
    return false;

  if A.size != B.size then
    return false;

  //
  // check B size/shape are the same to permit legal zippering
  //
  if isRectangularDom(A.domain) && isRectangularDom(B.domain) {
    for d in 1..A.rank do
      if A.domain.dim(d).size != B.domain.dim(d).size then
        return false;
  }

  //
  // if all the above tests match, see if zippered equality is
  // true everywhere
  //
  if isArrayType(A.eltType) {
    var ret = true;
    forall (Aarr, Barr) in zip(A, B) with (&& reduce ret) do
      ret &&= almostEquals(Aarr, Barr);
    return ret;
  } else {
    // TODO: Can A be written as a simple reduce?
    var ret = true;
    forall (Aelement, Belement) in zip(A, B) with (&& reduce ret) do
      ret &&= almostEquals(Aelement, Belement);
    return ret;
  }
}


/*
  Ideally, this would be supported with operator:

    A ~== B

  significant=7 is a better default, but 6 works well for our purposes
 */
proc almostEquals(a, b, significant=6) {
  return abs(b - a) <= 10.0**-(significant-1);
}
