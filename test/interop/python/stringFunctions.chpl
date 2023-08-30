use CTypes;
/* Tests accepting a chpl_c_string */
export proc takesString(in x: chpl_c_string) {
  writeln(string.createCopyingBuffer(x:c_ptrConst(c_char)));
}

/* Tests returning a chpl_c_string */
export proc getString(): chpl_c_string {
  var ret: chpl_c_string = "whee";
  return ret;
}

/* Tests taking and returning a chpl_c_string */
export proc takeAndReturn(in x: chpl_c_string): chpl_c_string {
  return x;
}
