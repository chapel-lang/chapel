class C {
  var xx: int = 0;
}

record R {
  var x: int = 0;
  var ptr: shared C = new shared C(0);
  proc init() {
    this.x = 0;
    this.ptr = new shared C(0);
    writeln("init (default)");
  }
  proc init(arg:int) {
    this.x = arg;
    this.ptr = new shared C(arg);
    writeln("init ", arg, " ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    this.ptr = new shared C(other.ptr.xx);
    writeln("init= ", other.x, " ", other.ptr.xx);
  }
  proc deinit() {
    writeln("deinit ", x, " ", ptr.xx);
  }
  proc toString() {
    return "(" + this.x:string + " " + this.ptr.xx:string + ")";
  }
  proc set1() {
    this.x = 1;
    this.ptr.xx = 1;
    return this;
  }
}
proc =(ref lhs:R, rhs:R) {
  writeln("lhs", lhs.toString(), " = rhs", rhs.toString());
  lhs.x = rhs.x;
  lhs.ptr = new shared C(rhs.ptr.xx);
}

proc makeR() {
  return new R(1);
}
proc makeR(arg: int) {
  return new R(arg);
}

proc out1(out arg: R) {
  arg = makeR();
}

proc test1() {
  writeln("test1");
  var x: R;
  out1(x);
  writeln(x);
}
test1();

proc out2(out arg: R) {
}

proc test2() {
  writeln("test2");
  var x: R;
  out2(x);
  writeln(x);
}
test2();

proc out3(out a: R, out b: R) {
  a = makeR(1);
  b = makeR(2);
  b = makeR(3);
}

proc test3() {
  writeln("test3");
  var x: R;
  out3(x, x);
  writeln(x);
}
test3();

proc inThenOut(in a: R, out b: R) {
  b = a;
}

proc test4() {
  writeln("test4");
  var x: R;
  inThenOut(x, x);
  writeln(x);
}
test4();

proc test5() {
  writeln("test5");
  var x: R;
  var y: R;
  inThenOut(x, y);
  writeln(y);
}
test5();
