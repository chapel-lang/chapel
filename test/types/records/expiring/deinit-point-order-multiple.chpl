record R {
  var x:int = 0;
  proc init(arg: int) {
    this.x = arg;
    writeln("init ", x);
  }
  proc deinit() {
    writeln("deinit ", x);
  }
}

proc sum1(a: R, b: R, c: R, d: R, e: R,
          f: R, g: R, h: R, i: R, j: R) {
  return a.x + b.x + c.x + d.x + e.x +
         f.x + g.x + h.x + i.x + j.x;
}

proc t1() {
  writeln("t1");
  var s = sum1(new R(1), new R(2), new R(3), new R(4), new R(5),
               new R(6), new R(7), new R(8), new R(9), new R(10));
  writeln("after last use of temporaries");
  writeln("sum1: ", s);
}
t1();

// Could return one of the records, so they are "captured"
proc choose(a: R, b: R, c: R, d: R, e: R,
            f: R, g: R, h: R, i: R, j: R) const ref {
  return j;
}


proc t2() {
  writeln("t2");
  const ref v = choose(new R(1), new R(2), new R(3), new R(4), new R(5),
                       new R(6), new R(7), new R(8), new R(9), new R(10));
  writeln("choose: ", v);
  writeln("at end of function");
}
t2();
