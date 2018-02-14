
{
  var a: real(64) = -100;
  var b: real(64) = -0.05;
  var c: real(64) = 0;
  var d: real(64) = 0.05;
  var e: real(64) = 100;

  writeln("For 64-bit reals: ");
  writeln("a = ", a);
  writeln("j0(a) = ", j0(a));
  writeln("b = ", b);
  writeln("j0(b) = ", j0(b));
  writeln("c = ", c);
  writeln("j0(c) = ", j0(c));
  writeln("d = ", d);
  writeln("j0(d) = ", j0(d));
  writeln("e = ", e);
  writeln("j0(e) = ", j0(e));
}

{
  writeln("For 32-bit reals: ");
  var a: real(32) = (-100):real(32);
  var b: real(32) = (-0.05):real(32);
  var c: real(32) = (0):real(32);
  var d: real(32) = (0.05):real(32);
  var e: real(32) = (100):real(32);

  writeln("a = ", a);
  writeln("j0(a) = ", j0(a));
  writeln("b = ", b);
  writeln("j0(b) = ", j0(b));
  writeln("c = ", c);
  writeln("j0(c) = ", j0(c));
  writeln("d = ", d);
  writeln("j0(d) = ", j0(d));
  writeln("e = ", e);
  writeln("j0(e) = ", j0(e));
}
