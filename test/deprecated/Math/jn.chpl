
{
  var a: real(64) = -100, an: int = 5;
  var b: real(64) = -0.05, bn: int = 4;
  var c: real(64) = 0, cn: int = 7;
  var d: real(64) = 0.05, dn: int = 2;
  var e: real(64) = 100, en: int = 9;

  writeln("For 64-bit reals: ");
  writeln("a = ", a, " an = ", an);
  writeln("jn(an, a) = ", jn(an, a));
  writeln("b = ", b, " bn = ", bn);
  writeln("jn(bn, b) = ", jn(bn, b));
  writeln("c = ", c, " cn = ", cn);
  writeln("jn(cn, c) = ", jn(cn, c));
  writeln("d = ", d, " dn = ", dn);
  writeln("jn(dn, d) = ", jn(dn, d));
  writeln("e = ", e, " en = ", en);
  writeln("jn(en, e) = ", jn(en, e));
}

{
  var a: real(32) = (-100):real(32), an: int = 5;
  var b: real(32) = (-0.05):real(32), bn: int = 4;
  var c: real(32) = (0):real(32), cn: int = 7;
  var d: real(32) = (0.05):real(32), dn: int = 2;
  var e: real(32) = (100):real(32), en: int = 9;

  writeln("a = ", a, " an = ", an);
  writeln("jn(an, a) = ", jn(an, a));
  writeln("b = ", b, " bn = ", bn);
  writeln("jn(bn, b) = ", jn(bn, b));
  writeln("c = ", c, " cn = ", cn);
  writeln("jn(cn, c) = ", jn(cn, c));
  writeln("d = ", d, " dn = ", dn);
  writeln("jn(dn, d) = ", jn(dn, d));
  writeln("e = ", e, " en = ", en);
  writeln("jn(en, e) = ", jn(en, e));
}
