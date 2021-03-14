/* Tests accepting a real */
export proc takesReal(x: real) {
  writeln(x);
}

/* Tests returning a real */
export proc getReal(): real {
  var ret: real = 1.5;
  return ret;
}

/* Tests taking and returning a real */
export proc takeAndReturn(x: real): real {
  return x + 1;
}
