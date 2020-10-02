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

config const size = 1;
const D = {1..size};

proc returnArrayUntyped() {
  var B:[1..size] R = [i in 1..size] new R(1);
  return B;
}

proc returnArrayTyped():[1..size] R {
  var B:[1..size] R = [i in 1..size] new R(1);
  return B;
}

proc returnArrayTypedDomain():[D] R {
  var B:[1..1] R = [i in 1..1] new R(1);
  return B;
}

proc testr11() {
  writeln("testr11");
  var B = returnArrayUntyped();
  writeln(B);
}
testr11();
writeln("-");

proc testr12() {
  writeln("testr12");
  var B:[1..size] R = returnArrayUntyped();
  writeln(B);
}
testr12();
writeln("-");

proc testr13() {
  writeln("testr13");
  var B:[D] R = returnArrayUntyped();
  writeln(B);
}
testr13();
writeln("-");

proc testr21() {
  writeln("testr21");
  var B = returnArrayTyped();
  writeln(B);
}
testr21();
writeln("-");

proc testr22() {
  writeln("testr22");
  var B:[1..size] R = returnArrayTyped();
  writeln(B);
}
testr22();
writeln("-");

proc testr23() {
  writeln("testr23");
  var B:[D] R = returnArrayTyped();
  writeln(B);
}
testr23();
writeln("-");

proc testr31() {
  writeln("testr31");
  var B = returnArrayTypedDomain();
  writeln(B);
}
testr31();
writeln("-");

proc testr32() {
  writeln("testr32");
  var B:[1..size] R = returnArrayTypedDomain();
  writeln(B);
}
testr32();
writeln("-");

proc testr33() {
  writeln("testr33");
  var B:[D] R = returnArrayTypedDomain();
  writeln(B);
}
testr33();
writeln("-");
