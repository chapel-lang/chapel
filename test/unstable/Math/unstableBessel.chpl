use Math;

var a: real(64) = -100, an: int = 5;
writeln("a = ", a, " an = ", an);
writeln("j0(a) = ", j0(a));
writeln("j1(a) = ", j1(a));
writeln("jn(an, a) = ", jn(an, a));

var b: real(32) = (-0.05):real(32), bn: int = 4;
writeln("b = ", b, " bn = ", bn);
writeln("j0(b) = ", j0(b));
writeln("j1(b) = ", j1(b));
writeln("jn(bn, b) = ", jn(bn, b));

var c: real(64) = 110;
writeln("c = ", c);
writeln("y0(c) = ", y0(c));
writeln("y1(c) = ", y1(c));

var d: real(32) = 0.05;
writeln("d = ", d);
writeln("y0(d) = ", y0(d));
writeln("y1(d) = ", y1(d));

var e: real(64) = 100, en: int = 9;
writeln("e = ", e, " en = ", en);
writeln("yn(en, e) = ", yn(en, e));

var f: real(32) = 31, fn: int = 4;
writeln("f = ", f, " fn = ", fn);
writeln("yn(fn, f) = ", yn(fn, f));
