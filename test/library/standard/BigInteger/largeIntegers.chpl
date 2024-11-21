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

testMul(10:bigint, max(uint));
testMul(10:bigint, max(int));
testMul(10:bigint, min(uint));
testMul(10:bigint, min(int));
