// Chapel file that exports functions: one that takes an complex, one that
// returns an complex, and one that does both.
export proc take(x: complex) {
  writeln(x);
}

export proc give(): complex {
  var ret: complex = 4 + 3i;
  return ret;
}

export proc takeAndReturn(x: complex): complex {
  return x;
}
