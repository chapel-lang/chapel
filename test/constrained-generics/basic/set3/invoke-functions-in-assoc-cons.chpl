// A test for invoking functions that are defined by an associated constraint
// from the default implementation of a required function.

interface i1(t1) {
  proc write(arg: t1): void;
  proc p1(arg1: t1);
}

interface i3(t3) {
  type a3;
  a3 implements i1;
  proc write(arg: t3): void;
  proc p3(arg3: a3) {
    write("i3.p3 "); write(arg3); writeln();
    // p1() is defined in the interface of the associated constraint
    p1(arg3);
  }
}

int implements i1;
proc p1(arg1: int) { writeln("p1.int ", arg1); }

real implements i3;
proc real.a3 type return int;

proc icFun(aa: ?Q, bb: aa.a3) where Q implements i3 {
  write("icFun "); write(aa); write(" "); write(bb); writeln();
  p3(bb);
}

icFun(23.32, 45);
