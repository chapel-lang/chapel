use BigInteger;

{
  var a: bigint = 17;
  var res: bigint;
  nextPrime(res, a);
  writeln("nextPrime ", res);
}

{
  var res: bigint;
  lucNum(res, 5);
  writeln("lucNum ", res);
}

{
  var res1: bigint;
  var res2: bigint;
  lucNum2(res1, res2, 7);
  writeln("lucNum2 ", (res1, res2));
}

{
  var a: bigint = 15;
  var b: bigint = 17;
  var res = jacobi(a, b);
  writeln("jacobi ", res);
}

{
  var a: bigint = 10;
  var b: bigint = 11;
  var res = legendre(a, b);
  writeln("legendre ", res);
}

{
  var a: bigint = 30;
  var b: bigint = 30;
  var res = kronecker(a, b);
  writeln("kronecker ", res);
  res = kronecker(a, 5:int);
  writeln("kronecker ", res);
  res = kronecker(10:int, b);
  writeln("kronecker ", res);
  res = kronecker(a, 7:uint);
  writeln("kronecker ", res);
  res = kronecker(11:uint, b);
  writeln("kronecker ", res);
}

{
  var a: bigint = 18;
  var b: bigint = 30;
  var res, s, t: bigint;
  gcd(res, a, b);
  writeln("gcd ", res);
  gcd(res, a, 33:int);
  writeln("gcd ", res);
  gcd(res, a, 36:uint);
  writeln("gcd ", res);
  gcd(res, a, b, s, t);
  writeln("gcd ", (res, s, t));
}

{
  var a: bigint = 15;
  var b: bigint = 30;
  var res: bigint;
  lcm(res, a, b);
  writeln("lcm ", res);
  lcm(res, a, 10:int);
  writeln("lcm ", res);
  lcm(res, a, 2:uint);
  writeln("lcm ", res);
}

{
  var res: bigint;
  var a = 6;
  fac(res, a);
  writeln("fac ", res);
}

{
  var res: bigint;
  var n: bigint = 3;
  bin(res, n, 7);
  writeln("bin ", res);
  bin(res, 9, 5);
  writeln("bin ", res);
}

{
  var res: bigint;
  var n = 7;
  fib(res, n);
  writeln("fib ", res);
}

{
  var res1: bigint;
  var res2: bigint;
  fib2(res1, res2, 12);
  writeln("fib2 ", (res1, res2));
}

{
  var a: bigint = 15;
  var res = a.probablyPrime(7);
  writeln("probablyPrime ", res);
}
