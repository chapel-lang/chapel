config var printInitDeinit = true;
  
class C {
  var xx: int = 0;
}

record R {
  var x: int = 0;
  var ptr: shared C = new shared C(0);
  proc init() {
    this.x = 0;
    this.ptr = new shared C(0);
    if printInitDeinit then writeln("init (default)");
  }
  proc init(arg:int) {
    this.x = arg;
    this.ptr = new shared C(arg);
    if printInitDeinit then writeln("init ", arg, " ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    this.ptr = new shared C(other.ptr.xx);
    if printInitDeinit then writeln("init= ", other.x, " ", other.ptr.xx);
  }
  proc deinit() {
    if printInitDeinit then writeln("deinit ", x, " ", ptr.xx);
  }
  proc toString() {
    return "(" + this.x:string + " " + this.ptr.xx:string + ")";
  }
  proc ref set1() ref {
    this.x = 1;
    this.ptr.xx = 1;
    return this;
  }
}
proc =(ref lhs:R, rhs:R) {
  if printInitDeinit then writeln("lhs", lhs.toString(), " = rhs", rhs.toString());
  lhs.x = rhs.x;
  lhs.ptr = new shared C(rhs.ptr.xx);
}

proc test1() {
  writeln("test1");
  var D: domain(int);
  var A:[D] R;
}
test1();

proc test2() {
  writeln("test2");
  var D: domain(int);
  var A:[D] R;
  D += 1;
}
test2();

proc test3() {
  writeln("test3");
  var D: domain(int);
  var A:[D] R;
  D += 1;
  D -= 1;
}
test3();

proc test4() {
  writeln("test4");
  var D: domain(int);
  var A:[D] R;
  D += 1;
  A[1] = new R(1);
}
test4();

proc test5() {
  writeln("test5");
  var D: domain(int);
  D += 1;
  var A:[D] R;
}
test5();

proc test6() {
  writeln("test6");
  var D: domain(int);
  D += 1;
  var A:[D] R;
  A[1] = new R(1);
}
test6();


// TODO: array from array literal, tuple, other array
