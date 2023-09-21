use BigInteger;

@deprecated("bigint.div_q using Round is deprecated, use the standalone function div with roundingMode instead")
proc ref bigint.div_q(x, y) {
  div(this, x, y);
}
