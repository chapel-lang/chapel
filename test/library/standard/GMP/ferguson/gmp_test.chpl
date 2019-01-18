use BigInteger;

config const neg = true;

var a = new bigint();

a.fac(100);
if neg then
  a.neg(a);

writeln("a=    ", a);

var x = new bigint("1838348473822929893829847293847289764918276498172649817264982619487612984761928476189274698126489127649");

writeln("x=    ", x);

var b = new bigint();

if neg then
  b.sub(b, 1);
else
  b.add(b, 1);

writeln("b=    ", b);

on Locales[numLocales - 1] {

  // check GET of 'a' through initializer
  var c = new bigint(a);
  writeln("c=    ", c);

  // check GET of 'a' in assignment
  var d = new bigint();
  d = a;
  writeln("d=a   ", d);

  // check GET of 'a' and 'b' in add
  var e = new bigint();
  e.add(a, b);
  writeln("e=a+b ", e);

  // check setting b (which requires resize)
  b.set(c);

  // check setting x from remote value
  x = a;
}

writeln("b=c   ", b);
writeln("x=a   ", x);
