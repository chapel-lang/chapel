/* Tests accepting a complex */
export proc takesComplex(x: complex) {
  writeln(x);
}

/* Tests returning a complex */
export proc getComplex(): complex {
  var ret: complex = 4 + 3i;
  return ret;
}

/* Tests taking and returning a complex */
export proc takeAndReturn(x: complex): complex {
  return x;
}
