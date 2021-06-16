// Check calling one IC function from another
// passing arguments of associated types.
// Exercise default intents and ref intents.
// Exercise the case of multiple interface constraints.

/////////////////////////////////////////////////////////////////////////////
// one interface, one constraint

interface I1(F1) {
  type AT;
  proc toString(arg: F1): string;
  proc toString(arg: AT): string;
  proc req1(arg: F1)     { writeln(" req1.F1  ", toString(arg)); }
  proc req1(arg: AT)     { writeln(" req1.AT  ", toString(arg)); }
  proc ref1(ref arg: F1) { writeln(" ref1.F1  ", toString(arg)); }
  proc ref1(ref arg: AT) { writeln(" ref1.AT  ", toString(arg)); }
}

proc ic1dd(x1: ?Q1, y1: x1.AT)
  where Q1 implements I1
{
  writeln("ic1dd     ", (toString(x1), toString(y1)));
  req1(x1);
  req1(y1);
}

proc ic1rd(ref x1: ?Q1, y1: x1.AT)
  where Q1 implements I1
{
  writeln("ic1rd     ", (toString(x1), toString(y1)));
  req1(x1);
  ref1(x1);
  req1(y1);
  ic1dd(x1, y1);
}

proc ic1dr(x1: ?Q1, ref y1: x1.AT)
  where Q1 implements I1
{
  writeln("ic1dr     ", (toString(x1), toString(y1)));
  req1(x1);
  req1(y1);
  ref1(y1);
  ic1dd(x1, y1);
}

proc ic1rr(ref x1: ?Q1, ref y1: x1.AT)
  where Q1 implements I1
{
  writeln("ic1rr     ", (toString(x1), toString(y1)));
  req1(x1);
  ref1(x1);
  req1(y1);
  ref1(y1);
  ic1dd(x1, y1);
  ic1rd(x1, y1);
  ic1dr(x1, y1);
}

real implements I1;
proc real.AT type return bool;
proc toString(arg): string return arg: string;

var m1 = 11.22;
var n1 = true;

ic1dd(22.11, false);     writeln();
ic1rd(m1,    false);     writeln();
ic1dr(22.33, n1);        writeln();
ic1rr(m1,    n1);        writeln();

/////////////////////////////////////////////////////////////////////////////
// two interfaces, two or three constraints

interface I2(F2) {
  type AT;
  proc toString(arg: F2): string;
  proc toString(arg: AT): string;
  proc req2(arg: F2)     { writeln(" req2.F2  ", toString(arg)); }
  proc req2(arg: AT)     { writeln(" req2.AT  ", toString(arg)); }
}

proc ic1ii1(x1: ?Q1, x2: ?Q2, y2: x2.AT, y1: x1.AT)
  where Q1 implements I1 && Q2 implements I1
{
  writeln("ic1ii1    ", (toString(x1), toString(y1),
                         toString(x2), toString(y2)));
  ic1dd(x1, y1);
  ic1dd(x2, y2);
}

proc ic1122(x1: ?Q1, y1: x1.AT, x2: ?Q2, y2: x2.AT)
  where Q1 implements I1 && Q2 implements I2
{
  writeln("ic1122    ", (toString(x1), toString(y1),
                         toString(x2), toString(y2)));
  req2(x2);
  req2(y2);
  ic1dd(x1, y1);
  ic1ii1(x1, x1, y1, y1);
}

proc ic12i2i1(x1: ?Q1, x2: ?Q2, x3: ?Q3, y2: x2.AT, y3: x3.AT, y1: x1.AT)
  where Q1 implements I1 && Q2 implements I2 && Q3 implements I1
{
  writeln("ic12i2i1  ", (toString(x1), toString(y1),
                         toString(x2), toString(y2),
                         toString(x3), toString(y3)));
  req2(x2);
  req2(y2);
  ic1dd(x1, y1);
  ic1dd(x3, y3);
  ic1ii1(x1, x3, y3, y1);
  ic1122(x1, y1, x2, y2);
  ic1122(x3, y3, x2, y2);
}

imag implements I1;
proc imag.AT type return complex;
string implements I2;
proc string.AT type return int;

ic1ii1(1.1, 1.2, false, true);                  writeln();
ic1ii1(2.1, 2i, 2+1i, false);                   writeln();
ic1122(3.1, true, "halo", 333);                 writeln();
ic12i2i1(4.1, "wow", 4i, 444, 4+1i, false);     writeln();

/////////////////////////////////////////////////////////////////////////////
// done

writeln("done");
