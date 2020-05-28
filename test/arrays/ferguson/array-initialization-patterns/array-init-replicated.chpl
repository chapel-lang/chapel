use ReplicatedDist;

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


proc makeInitialArray() {
  var D = {1..1} dmapped Replicated();
  var ret: [D] int;
  return ret;
}

var A = makeInitialArray();
writeln("initial array is ", A);
writeln("-");

proc test0() {
  writeln("test0");
  var B = A;
  writeln(A, " ", B);
}
A = 0;
test0();

proc test1() {
  writeln("test1");
  var B:[1..1] int = A;
  writeln(A, " ", B);
}
A = 0;
test1();

proc test2() {
  writeln("test2");
  var B:A.type = A;
  writeln(A, " ", B);
}
A = 0;
test2();

proc test3() {
  writeln("test3");
  var B:[A.domain] int = A;
  writeln(A, " ", B);
}
A = 0;
test3();

proc test4() {
  writeln("test4");
  var B:[A.domain] int = 1..1;
  writeln(B);
}
A = 0;
test4();
  
proc test5() {
  writeln("test5");
  var B:[A.domain] int = [i in 1..1] i;
  writeln(B);
}
A = 0;
test5();

proc test6() {
  writeln("test6");
  var B:[A.domain] int = {1..1};
  writeln(B);
}
A = 0;
test6();

proc test7() {
  writeln("test7");
  var B:[A.domain] int = 1;
  writeln(B);
}
A = 0;
test7();

proc test8() {
  writeln("test8");
  var B:[A.domain] int = (1,);
  writeln(B);
}
A = 0;
test8();

proc testrDefault() {
  writeln("testrDefault");
  var B:[A.domain] R;
  writeln(B);
}
testrDefault();
writeln("-");

proc makeA() {
  var savePrint = printInitDeinit;
  printInitDeinit = false;
  var ret: [A.domain] R;
  ret[1] = new R(1);
  printInitDeinit = savePrint;
  if printInitDeinit then
    writeln("init 1 1");
  return ret;
}

printInitDeinit = false;
var globalA = makeA();
printInitDeinit = true;
writeln("globalA is ", globalA);
writeln("-");

printInitDeinit = false;
