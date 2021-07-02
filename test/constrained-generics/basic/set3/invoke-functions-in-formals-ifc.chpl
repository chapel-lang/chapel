// A test for invoking functions that are defined by the interface
// of an IC formal.

interface i1(t1) {
  proc write(arg: t1): void;
  proc p1(arg1: t1): void;
}

interface i2(t2) {
  proc write(arg: t2): void;
  proc p2(xx: t2, yy: ?Q2): void
    where Q2 implements i1
  {
    write("i2.p2 "); write(xx); write(" "); write(yy); writeln();
    // p1() is defined in the interface of the IC formal 'yy'
    p1(yy);
  }
}

proc icFun(kk: ?F1, ll: ?F2)
  where F1 implements i1
     && F2 implements i2
{
  write("icFun "); write(kk); write(" "); write(ll); writeln();
  p2(ll, kk);
  p2(ll, 2221);
}

int implements i1;
proc p1(arg1: int) { writeln("p1.int ", arg1); }

real implements i2;  // using the default for p2()

icFun(2220, 22.24);
