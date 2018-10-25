/* Tests accepting a c_string */
export proc takesString(x: c_string) {
  writeln(x: string);
}

/* Tests returning a c_string */
export proc getString(): c_string {
  var ret: c_string = "whee";
  return ret;
}

/* Tests taking and returning a c_string */
export proc takeAndReturn(x: c_string): c_string {
  return x;
}
