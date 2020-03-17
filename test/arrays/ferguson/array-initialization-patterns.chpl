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


var A:[1..1] int;

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
  var B:[1..1] int = 1..1;
  writeln(B);
}
A = 0;
test4();
  
proc test5() {
  writeln("test5");
  var B:[1..1] int = [i in 1..1] i;
  writeln(B);
}
A = 0;
test5();

proc test6() {
  writeln("test6");
  var B:[1..1] int = {1..1};
  writeln(B);
}
A = 0;
test6();

proc test7() {
  writeln("test7");
  var B:[1..1] int = 1;
  writeln(B);
}
A = 0;
test7();

proc test8() {
  writeln("test8");
  var B:[1..1] int = (1,);
  writeln(B);
}
A = 0;
test8();

proc makeA() {
  var savePrint = printInitDeinit;
  printInitDeinit = false;
  var ret: [1..1] R;
  ret[1] = new R(1);
  printInitDeinit = savePrint;
  if printInitDeinit then
    writeln("init 1 1");
  return ret;
}

printInitDeinit = false;
var globalA = makeA();
printInitDeinit = true;
writeln("-");

proc testr0() {
  writeln("testr0");
  var B = makeA();
  writeln(B);
}
testr0();
writeln("-");

proc testr1() {
  writeln("testr1");
  var B:[1..1] R = globalA;
  writeln(B);
}
testr1();
writeln("-");

proc testr2() {
  writeln("testr2");
  var B:[1..1] R = makeA();
  writeln(B);
}
testr2();
writeln("-");

proc testr3() {
  writeln("testr3");
  var B:[1..1] R = (new R(1),);
  writeln(B);
}
testr3();
writeln("-");

proc testr4() {
  writeln("testr4");
  var B:[1..10] R = new R(1);
  writeln(B);
}
testr4();
writeln("-");

proc testr5() {
  writeln("testr5");
  var B:[1..1] R = [i in 1..1] new R(1);
  writeln(B);
}
testr5();
writeln("-");


printInitDeinit = false;
