
{
  var a: real(64) = -100;
  var b: real(64) = -0.05;
  var c: real(64) = 0;
  var d: real(64) = 0.05;
  var e: real(64) = 100;

  writeln("For 64-bit reals: ");
  writeln("a = ", a);
  writeln("j1(a) = ", j1(a));
  writeln("b = ", b);
  writeln("j1(b) = ", j1(b));
  writeln("c = ", c);
  writeln("j1(c) = ", j1(c));
  writeln("d = ", d);
  writeln("j1(d) = ", j1(d));
  writeln("e = ", e);
  writeln("j1(e) = ", j1(e));
}

{
  writeln("For 32-bit reals: ");
  var a: real(32) = (-100):real(32);
  var b: real(32) = (-0.05):real(32);
  var c: real(32) = (0):real(32);
  var d: real(32) = (0.05):real(32);
  var e: real(32) = (100):real(32);

  writeln("a = ", a);
  writeln("j1(a) = ", j1(a));
  writeln("b = ", b);
  writeln("j1(b) = ", j1(b));
  writeln("c = ", c);
  writeln("j1(c) = ", j1(c));
  writeln("d = ", d);
  writeln("j1(d) = ", j1(d));
  writeln("e = ", e);
  writeln("j1(e) = ", j1(e));
}
