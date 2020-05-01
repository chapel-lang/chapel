/* Tests accepting a c_string */
export proc takesString(in x: c_string) {
  writeln(createStringWithNewBuffer(x));
}

/* Tests returning a c_string */
export proc getString(): c_string {
  var ret: c_string = "whee";
  return ret;
}

/* Tests taking and returning a c_string */
export proc takeAndReturn(in x: c_string): c_string {
  return x;
}
