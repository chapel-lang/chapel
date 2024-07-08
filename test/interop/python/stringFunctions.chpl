use CTypes;

/* Tests accepting a chpl_c_string */
export proc takesStringC(in x: chpl_c_string) {
  writeln(string.createCopyingBuffer(x:c_ptrConst(c_char)));
}

/* Tests returning a chpl_c_string */
export proc getStringC(): chpl_c_string {
  var ret: c_ptrConst(c_char) = "whee";
  return ret;
}

/* Tests taking and returning a chpl_c_string */
export proc takeAndReturnC(in x: chpl_c_string): chpl_c_string {
  return x;
}

/* Tests accepting a c_ptrConst(c_char) */
export proc takesString(in x: c_ptrConst(c_char)) {
  writeln(string.createCopyingBuffer(x));
}

/* Tests returning a c_ptrConst(c_char) */
export proc getString(): c_ptrConst(c_char) {
  var ret: c_ptrConst(c_char) = "whee";
  return ret;
}

/* Tests taking and returning a c_ptrConst(c_char) */
export proc takeAndReturn(in x: c_ptrConst(c_char)): c_ptrConst(c_char) {
  return x;
}
