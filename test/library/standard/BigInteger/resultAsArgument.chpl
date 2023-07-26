use BigInteger;

{
  write("divExact ");
  var a = new bigint(12);
  var b = new bigint(6);
  divExact(a, a, b);
  writeln(a);
}
{
  write("divExact ");
  var a = new bigint(12);
  divExact(a, a, 6);
  writeln(a);
}
{
  write("powMod ");
  var a = new bigint(4);
  var b = new bigint(2);
  var c = new bigint(5);
  powMod(a, a, b, c);
  writeln(a);
}
{
  write("powMod ");
  var a = new bigint(4);
  var b = new bigint(2);
  var c = new bigint(5);
  powMod(b, a, b, c);
  writeln(b);
}
{
  write("powMod ");
  var a = new bigint(4);
  var b = new bigint(2);
  var c = new bigint(5);
  powMod(c, a, b, c);
  writeln(c);
}
{
  write("powMod ");
  var a = new bigint(4);
  var b = new bigint(5);
  powMod(a, a, 2:uint, b);
  writeln(a);
}
{
  write("powMod ");
  var a = new bigint(4);
  var b = new bigint(5);
  powMod(b, a, 2:uint, b);
  writeln(b);
}
{
  write("pow ");
  var a = new bigint(4);
  pow(a, a, -2);
  writeln(a);
}
{
  write("pow ");
  var a = new bigint(4);
  pow(a, a, 2:uint);
  writeln(a);
}
{
  write("root ");
  var a = new bigint(4);
  root(a, a, 2:uint);
  writeln(a);
}
{
  write("rootrem ");
  var a = new bigint(5);
  var b: bigint;
  rootrem(a, b, a, 2:uint);
  writeln(a, " ", b);
}
{
  write("rootrem ");
  var a = new bigint(5);
  var b: bigint;
  rootrem(b, a, a, 2:uint);
  writeln(b, " ", a);
}
{
  write("sqrt ");
  var a = new bigint(9);
  sqrt(a, a);
  writeln(a);
}
{
  write("sqrtrem ");
  var a = new bigint(10);
  var b: bigint;
  sqrtrem(a, b, a);
  writeln(a, " ", b);
}
{
  write("sqrtrem ");
  var a = new bigint(10);
  var b: bigint;
  sqrtrem(b, a, a);
  writeln(b, " ", a);
}
{
  write("nextPrime ");
  var a = new bigint(16);
  nextPrime(a, a);
  writeln(a);
}
{
  write("gcd ");
  var a = new bigint(8);
  var b = new bigint(12);
  gcd(a, a, b);
  writeln(a);
}
{
  write("gcd ");
  var a = new bigint(8);
  var b = new bigint(12);
  gcd(b, a, b);
  writeln(b);
}
{
  write("gcd ");
  var a = new bigint(8);
  gcd(a, a, 12:uint);
  writeln(a);
}
{
  write("gcd ");
  var a = new bigint(8);
  var b = new bigint(12);
  var c: bigint;
  var d: bigint;
  gcd(a, a, b, c, d);
  writeln(a, " ", c, " ", d);
}
{
  write("gcd ");
  var a = new bigint(8);
  var b = new bigint(12);
  var c: bigint;
  var d: bigint;
  gcd(b, a, b, c, d);
  writeln(b, " ", c, " ", d);
}
{
  write("gcd ");
  var a = new bigint(8);
  var b = new bigint(12);
  var c: bigint;
  gcd(c, a, b, a, b);
  writeln(c, " ", a, " ", b);
}
{
  write("lcm ");
  var a = new bigint(4);
  var b = new bigint(6);
  lcm(a, a, b);
  writeln(a);
}
{
  write("lcm ");
  var a = new bigint(4);
  var b = new bigint(6);
  lcm(b, a, b);
  writeln(b);
}
{
  write("lcm ");
  var a = new bigint(4);
  lcm(a, a, 6:uint);
  writeln(a);
}
{
  write("invert ");
  var a = new bigint(5);
  var b = new bigint(13);
  invert(a, a, b);
  writeln(a);
}
{
  write("invert ");
  var a = new bigint(5);
  var b = new bigint(13);
  invert(b, a, b);
  writeln(b);
}
{
  write("removeFactor ");
  var a = new bigint(5);
  var b = new bigint(13);
  removeFactor(a, a, b);
  writeln(a);
}
{
  write("removeFactor ");
  var a = new bigint(5);
  var b = new bigint(13);
  removeFactor(b, a, b);
  writeln(b);
}
{
  write("bin ");
  var a = new bigint(2);
  bin(a, a, 1);
  writeln(a);
}
{
  write("add ");
  var a = new bigint(5);
  var b = new bigint(6);
  add(a, a, b);
  writeln(a);
}
{
  write("add ");
  var a = new bigint(5);
  var b = new bigint(6);
  add(b, a, b);
  writeln(b);
}
{
  write("add ");
  var a = new bigint(5);
  add(a, a, 6:uint);
  writeln(a);
}
{
  write("add ");
  var a = new bigint(5);
  add(a, a, -6);
  writeln(a);
}
{
  write("sub ");
  var a = new bigint(5);
  var b = new bigint(6);
  sub(a, a, b);
  writeln(a);
}
{
  write("sub ");
  var a = new bigint(5);
  var b = new bigint(6);
  sub(b, a, b);
  writeln(b);
}
{
  write("sub ");
  var a = new bigint(5);
  sub(a, a, 6:uint);
  writeln(a);
}
{
  write("sub ");
  var a = new bigint(5);
  sub(a, -6, a);
  writeln(a);
}
{
  write("sub ");
  var a = new bigint(5);
  sub(a, 6:uint, a);
  writeln(a);
}
{
  write("mul ");
  var a = new bigint(5);
  var b = new bigint(6);
  mul(a, a, b);
  writeln(a);
}
{
  write("mul ");
  var a = new bigint(5);
  var b = new bigint(6);
  mul(b, a, b);
  writeln(b);
}
{
  write("mul ");
  var a = new bigint(5);
  mul(a, a, 6:int);
  writeln(a);
}
{
  write("mul ");
  var a = new bigint(5);
  mul(a, a, 6:uint);
  writeln(a);
}
{
  write("addmul ");
  var a = new bigint(5);
  var b = new bigint(6);
  addmul(a, a, b);
  writeln(a);
}
{
  write("addmul ");
  var a = new bigint(5);
  var b = new bigint(6);
  addmul(b, a, b);
  writeln(b);
}
{
  write("addmul ");
  var a = new bigint(5);
  addmul(a, a, 6:uint);
  writeln(a);
}
{
  write("submul ");
  var a = new bigint(5);
  var b = new bigint(6);
  submul(a, a, b);
  writeln(a);
}
{
  write("submul ");
  var a = new bigint(5);
  var b = new bigint(6);
  submul(b, a, b);
  writeln(b);
}
{
  write("submul ");
  var a = new bigint(5);
  submul(a, a, 6:uint);
  writeln(a);
}
{
  write("mul2Exp ");
  var a = new bigint(5);
  mul2Exp(a, a, 2);
  writeln(a);
}
{
  write("neg ");
  var a = new bigint(5);
  neg(a, a);
  writeln(a);
}
{
  write("abs ");
  var a = new bigint(-5);
  abs(a, a);
  writeln(a);
}
{
  write("div ");
  var a = new bigint(10);
  var b = new bigint(2);
  div(a, a, b);
  writeln(a);
}
{
  write("div ");
  var a = new bigint(10);
  var b = new bigint(2);
  div(b, a, b);
  writeln(b);
}

{
  write("rem ");
  var a = new bigint(10);
  var b = new bigint(2);
  rem(a, a, b);
  writeln(a);
}
{
  write("rem ");
  var a = new bigint(10);
  var b = new bigint(2);
  rem(b, a, b);
  writeln(b);
}
{
  write("divRem ");
  var a = new bigint(10);
  var b = new bigint(2);
  divRem(a, b, a, b);
  writeln(a, " ", b);
}
{
  write("divRem ");
  var a = new bigint(10);
  var b = new bigint(2);
  divRem(b, a, a, b);
  writeln(b, " ", a);
}
{
  write("div2Exp ");
  var a = new bigint(3);
  div2Exp(a, a, 1);
  writeln(a);
}
{
  write("rem2Exp ");
  var a = new bigint(3);
  rem2Exp(a, a, 1);
  writeln(a);
}
{
  write("mod ");
  var a = new bigint(10);
  var b = new bigint(3);
  mod(a, a, b);
  writeln(a);
}
{
  write("mod ");
  var a = new bigint(10);
  var b = new bigint(3);
  mod(b, a, b);
  writeln(b);
}
{
  write("mod ");
  var a = new bigint(10);
  mod(a, a, 3);
  writeln(a);
}
{
  write("and ");
  var a = new bigint(5);
  var b = new bigint(6);
  and(a, a, b);
  writeln(a);
}
{
  write("and ");
  var a = new bigint(5);
  var b = new bigint(6);
  and(b, a, b);
  writeln(b);
}
{
  write("ior ");
  var a = new bigint(5);
  var b = new bigint(6);
  ior(a, a, b);
  writeln(a);
}
{
  write("ior ");
  var a = new bigint(5);
  var b = new bigint(6);
  ior(b, a, b);
  writeln(b);
}
{
  // test the swap behavior where op2 has more limbs than op1
  write("ior ");
  var a = new bigint(10);
  var b = new bigint("1267650600228229401496703205376"); // 2**100
  ior(a, a, b);
  writeln(a);
}
{
  write("xor ");
  var a = new bigint(5);
  var b = new bigint(6);
  xor(a, a, b);
  writeln(a);
}
{
  write("xor ");
  var a = new bigint(5);
  var b = new bigint(6);
  xor(b, a, b);
  writeln(b);
}
{
  write("com ");
  var a = new bigint(5);
  com(a, a);
  writeln(a);
}
