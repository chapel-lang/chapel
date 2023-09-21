// deprecated in 1.32 by Jade

use BigInteger;


{
  compilerWarning("mod");
  writeln("mod");
  var a: bigint = 15;
  var b: bigint = 6;
  var res: bigint;
  mod(res, a=a, b=b); // warn
  writeln(res);
  mod(res, a=a, b=3); // warn
  writeln(res);
  mod(res, x=a, y=b);
  writeln(res);
  mod(res, x=a, y=-4);
  writeln(res);
  mod(res, a, b);
  writeln(res);
  mod(res, a, 2);
  writeln(res);
}

{
  compilerWarning("init");
  writeln("init");
  var x: bigint = 10;

  var init1 = new bigint(num=x);
  writeln(init1);
  var init2 = new bigint(num=10:int);
  writeln(init2);
  var init3 = new bigint(num=10:uint);
  writeln(init3);
  var init4 = new bigint(str="10");
  writeln(init4);
  var init5 = new bigint(str="FF", 16);
  writeln(init5);
}

{
  compilerWarning("root/sqrt");
  writeln("root/sqrt");
  var a: bigint = 27;
  var res: bigint;

  root(res, a=a, n=3:uint);
  writeln(res);
  sqrt(res, a=a);
  writeln(res);
}

{
  compilerWarning("invert");
  writeln("invert");
  var a: bigint = 7;
  var b: bigint = 17;
  var res: bigint;

  invert(res, a=a, b=b);
  writeln(res);
}

{
  compilerWarning("add");
  writeln("add");
  var a: bigint = 17;
  var b: bigint = 11;
  var res: bigint;

  add(res, a=a, b=b);
  writeln(res);
  add(res, a=a, b=10);
  writeln(res);
  add(res, a=a, b=9:uint);
  writeln(res);
}

{
  compilerWarning("sub");
  writeln("sub");
  var a: bigint = 17;
  var b: bigint = 11;
  var res: bigint;

  sub(res, a=a, b=b);
  writeln(res);
  sub(res, a=a, b=9);
  writeln(res);
  sub(res, a=a, b=13:uint);
  writeln(res);
  sub(res, a=6, b=b);
  writeln(res);
  sub(res, a=7:uint, b=b);
  writeln(res);
}

{
  compilerWarning("mul");
  writeln("mul");
  var a: bigint = 17;
  var b: bigint = 2;
  var res: bigint;

  mul(res, a=a, b=b);
  writeln(res);
  mul(res, a=a, b=-1);
  writeln(res);
  mul(res, a=a, b=3:uint);
  writeln(res);
}

{
  compilerWarning("neg/abs");
  writeln("neg/abs");
  var a: bigint = 17;
  var res: bigint;

  neg(res, a=a);
  writeln(res);
  abs(res, a=a);
  writeln(res);
}

{
  compilerWarning("cmp");
  writeln("cmp");
  var a: bigint = 17;
  var r: int;

  r = a.cmp(b=16:bigint);
  writeln(r);
  r = a.cmp(b=17);
  writeln(r);
  r = a.cmp(b=18:uint);
  writeln(r);
  r = a.cmp(b=17:real);
  writeln(r);
}

{
  compilerWarning("cmpabs");
  writeln("cmpabs");
  var a: bigint = -17;
  var r: int;

  r = a.cmpabs(b=16:bigint);
  writeln(r);
  r = a.cmpabs(b=18:uint);
  writeln(r);
  r = a.cmpabs(b=17:real);
  writeln(r);
}

{
  compilerWarning("and/xor/com");
  writeln("and/xor/com");
  var a: bigint = 17;
  var b: bigint = 2;
  var res: bigint;

  and(res, a=a, b=b);
  writeln(res);
  com(res, a=a);
  writeln(res);
  xor(res, a=a, b=b);
  writeln(res);
}

{
  compilerWarning("set");
  writeln("set");
  var a: bigint;

  a.set(a=17:bigint);
  writeln(a);
  a.set(num=18);
  writeln(a);
  a.set(num=19:uint);
  writeln(a);
  a.set(num=20:real);
  writeln(a);
  a.set(str="21");
  writeln(a);
}

{
  compilerWarning("swap");
  writeln("swap");
  var a: bigint = 17;
  var b: bigint = 19;

  a.swap(a=b);
  writeln((a,b));
}
