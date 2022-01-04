config param printInitDeinit = true;

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
  proc postinit() {
    if printInitDeinit then writeln("postinit ", x, " ", ptr.xx);
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
operator R.=(ref lhs:R, rhs:R) {
  if printInitDeinit then writeln("lhs", lhs.toString(), " = rhs", rhs.toString());
  lhs.x = rhs.x;
  lhs.ptr = new shared C(rhs.ptr.xx);
}

proc test100() {
  writeln("test100");
  var x = new R(1);
  serial {
    var y = x;
  }
}
test100();

proc test101() {
  writeln("test101");
  serial {
    var x = new R(1);
    var y = x;
  }
}
test101();

proc test200() {
  writeln("test200");
  var x = new R(1);
  local {
    var y = x;
  }
}
test200();

proc test201() {
  writeln("test201");
  local {
    var x = new R(1);
    var y = x;
  }
}
test201();
