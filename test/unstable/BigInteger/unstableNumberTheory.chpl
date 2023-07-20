use BigInteger;

{
  var a: bigint = 17;
  var res: bigint;
  nextPrime(res, a);
  writeln(res);
}

{
  var res: bigint;
  lucNum(res, 5);
  writeln(res);
}

{
  var res1: bigint;
  var res2: bigint;
  lucNum2(res1, res2, 7);
  writeln((res1, res2));
}
