use BigInteger;

var a = new Bigint();

a.fac(100);

writeln(a);

var x = new Bigint("1838348473822929893829847293847289764918276498172649817264982619487612984761928476189274698126489127649");

writeln(x);

var b = new Bigint(a);

b.add(b, 1);

on Locales[numLocales - 1] {
  var c = new Bigint(a);
  var d = new Bigint();

  d.add(c, b);

  writeln(d);

  b.set(d);
}

writeln(b);
