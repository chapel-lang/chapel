use CTypes;
/* Tests accepting a c_ptrConst(c_char) */
export proc takesString(in x: c_ptrConst(c_char)) {
  writeln(string.createCopyingBuffer(x:c_ptrConst(c_char)));
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
