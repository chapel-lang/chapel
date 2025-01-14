use BigInteger;

// test that Chapel int/uint is converted properly

proc testMul(x, y) {
  writeln(x, " * ", y);
  var res: bigint;
  mul(res, x, y);
  writeln("  mul: ", res);

  res = 0;
  res = x * y;
  writeln("    *: ", res);

  res = x;
  res *= y;
  writeln("   *=: ", res);
}

import ChplConfig;
param intSize = if ChplConfig.CHPL_TARGET_PLATFORM == "linux32" then 32 else 64;
// extra casts due to https://github.com/chapel-lang/chapel/issues/26533
testMul(10:bigint, max(uint(intSize)):uint);
testMul(10:bigint, max(int(intSize)):int);
testMul(10:bigint, min(uint(intSize)):uint);
testMul(10:bigint, min(int(intSize)):int);
