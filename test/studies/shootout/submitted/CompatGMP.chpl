use BigInteger;

@deprecated("bigint.div_q using Round is deprecated, use the standalone function div with roundingMode instead")
proc ref bigint.div_q(x, y) {
  div(this, x, y);
}

@deprecated("bigint.add method is deprecated - please use the standalone function add")
proc ref bigint.add(x, y) do BigInteger.add(this, x, y);

@deprecated("bigint.sub method is deprecated - please use the standalone function sub")
proc ref bigint.sub(x, y) do BigInteger.sub(this, x, y);

@deprecated("bigint.mul method is deprecated - please use the standalone function mul")
proc ref bigint.mul(x, y) do BigInteger.mul(this, x, y);

@deprecated("bigint.addmul method is deprecated - please use the standalone function addMul instead")
proc ref bigint.addmul(x, y) do BigInteger.addMul(this, x, y);

@deprecated("bigint.submul method is deprecated - please use the standalone function subMul instead")
proc ref bigint.submul(x, y) do BigInteger.subMul(this, x, y);

@deprecated("bigint.divQ method is deprecated - please use the standalone function div with roundingMode instead")
proc ref bigint.divQ(x, y) do BigInteger.div(this, x, y);
