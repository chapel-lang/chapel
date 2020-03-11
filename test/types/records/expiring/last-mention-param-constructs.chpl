class C { }
record R {
  var x: int = 0;
  var ptr: owned C = new owned C();
  proc init() {
    this.x = 0;
    writeln("init");
  }
  proc init(arg:int) {
    this.x = arg;
    writeln("init ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    writeln("init= ", other.x);
  }
  proc deinit() {
    writeln("deinit ", x);
  }
}
proc =(ref lhs:R, rhs:R) {
  writeln("= ", lhs.x, " ", rhs.x);
  lhs.x = rhs.x;
}


proc test1() {
  writeln("test1");
  var r = new R(1);

  writeln(r);

  // point 1

  writeln("before param conditional");

  if false {
    writeln(r);
  }

  // point 2

  writeln("after param conditional");
}
test1();

proc test2() {
  writeln("test2");
  var r = new R(1);

  writeln(r);

  // point 1

  writeln("before param loop");

  for param i in 1..2 {
    writeln(r);
  }

  // point 2

  writeln("after param loop");
}
test2();

proc test3() {
  writeln("test3");
  var r = new R(1);

  writeln(r);

  // point 1

  writeln("before param loop");

  for param i in 1..0 {
    writeln(r);
  }

  // point 2

  writeln("after param loop");
}
test3();

inline proc f(arg) { }

proc test4() {
  writeln("test4");
  var r = new R(1);

  writeln(r);

  // point 1

  writeln("before inlined empty call");

  f(r);

  // point 2

  writeln("after inlined empty call");
}
test4();

proc test5() {
  writeln("test5");
  var r = new R(1);

  writeln(r);

  // point 1

  writeln("before folded if-expr in call");

  f(if true then 1 else r);

  // point 2

  writeln("after folded if-expr in call");
}
test5();
