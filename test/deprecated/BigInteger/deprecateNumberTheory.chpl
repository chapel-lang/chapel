use BigInteger;

{
  var a: bigint = 5;
  var res: bigint;
  nextprime(res, a);
  writeln(res);
}

{
  var res: bigint;
  lucnum(res, 2);
  writeln(res);
}

{
  var res1: bigint;
  var res2: bigint;
  lucnum2(res1, res2, 4);
  writeln((res1, res2));
}
