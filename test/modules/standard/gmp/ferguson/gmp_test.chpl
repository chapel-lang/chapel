use BigIntegers;

var a = new BigInt();

a.fac(100);

writeln(a);

var x = new BigInt("1838348473822929893829847293847289764918276498172649817264982619487612984761928476189274698126489127649");
writeln(x);

//a.debugprint();

var b = new BigInt(a);
b.add(b, 1);
//b.debugprint();

on Locales[numLocales-1] {
  var c = new BigInt(a);
  //c.debugprint();
  var d = new BigInt();
  d.add(c,b);
  //d.debugprint();
  writeln(d);
  b.set(d);
}

//b.debugprint();
writeln(b);
