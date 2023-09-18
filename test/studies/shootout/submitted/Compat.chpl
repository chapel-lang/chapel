use IO, BigInteger, Regex;

@deprecated("openfd is deprecated, please use the file initializer with a 'c_int' argument instead")
proc openfd(x) {
  return new file(x);
}

@deprecated("bigint.div_q using Round is deprecated, use the standalone function div with roundingMode instead")
proc ref bigint.div_q(x, y) {
  div(this, x, y);
}

@deprecated("'Regex.compile' is deprecated. Please use 'new regex()' instead.")
proc compile(x) {
  return new regex(x);
}

@deprecated("regex.sub is deprecated. Please use string.replace.")
proc regex.sub(x, y) {
  return y.replace(this, x);
}