config const option = true;
config param paramOption = true;

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

proc inout1(inout arg: R) {
  arg = makeR();
}

proc test1() {
  writeln("test1");
  var x: R;
  inout1(x);
  writeln(x);
}
test1();

proc inout1a(inout arg: R) {
  writeln(arg);
  arg = makeR();
}

proc test1a() {
  writeln("test1a");
  var x: R;
  inout1a(x);
  writeln(x);
}
test1a();

proc inout2(inout arg: R) {
}

proc test2() {
  writeln("test2");
  var x: R;
  inout2(x);
  writeln(x);
}
test2();

proc inout3(inout a: R, inout b: R) {
  a = makeR(1);
  b = makeR(2);
  b = makeR(3);
}

proc test3() {
  writeln("test3");
  var x: R;
  inout3(x, x);
  writeln(x);
}
test3();

proc inThenInOut(in a: R, inout b: R) {
  b = a;
}

proc test4() {
  writeln("test4");
  var x: R;
  inThenInOut(x, x);
  writeln(x);
}
test4();

proc test5() {
  writeln("test5");
  var x: R;
  var y: R;
  inThenInOut(x, y);
  writeln(y);
}
test5();

proc inoutGenericArray1(inout arr) {
}

proc test6() {
  writeln("test6");
  var AA:[1..1] R;
  inoutGenericArray1(AA);
  writeln(AA);
}
test6();

proc inoutGenericArray2(inout arr) {
  var B:[1..1] R;
  B[1] = makeR(1);
  arr = B;
}

proc test7() {
  writeln("test7");
  var AA:[1..1] R;
  inoutGenericArray2(AA);
  writeln(AA);
}
test7();

proc inoutGenericArray2a(inout arr) {
  writeln(arr.domain, " ", arr); // preventing split-init
  var B:[1..1] R;
  B[1] = makeR(1);
  arr = B;
}

proc test7a() {
  writeln("test7a");
  var AA:[1..1] R;
  inoutGenericArray2a(AA);
  writeln(AA);
}
test7a();

proc inoutPartialArray1(inout arr:[] R) {
}

proc test8() {
  writeln("test8");
  var AA:[1..1] R;
  inoutPartialArray1(AA);
  writeln(AA);
}
test8();

proc inoutPartialArray2(inout arr:[] R) {
  var B:[1..1] R;
  B[1] = makeR(1);
  arr = B;
}

proc test9() {
  writeln("test9");
  var AA:[1..1] R;
  inoutPartialArray2(AA);
  writeln(AA);
}
test9();


proc inoutConcreteArray1(inout arr:[1..1] R) {
}

proc test10() {
  writeln("test10");
  var AA:[1..1] R;
  inoutConcreteArray1(AA);
  writeln(AA);
}
test10();

proc inoutConcreteArray2(inout arr:[1..1] R) {
  var B:[1..1] R;
  B[1] = makeR(1);
  arr = B;
}

proc test11() {
  writeln("test11");
  var AA:[1..1] R;
  inoutConcreteArray2(AA);
  writeln(AA);
}
test11();

proc inoutConcreteArray2a(inout arr:[1..1] R) {
  writeln(arr.domain, " ", arr); // preventing split init
  var B:[1..1] R;
  B[1] = makeR(1);
  arr = B;
}

proc test11a() {
  writeln("test11a");
  var AA:[1..1] R;
  inoutConcreteArray2a(AA);
  writeln(AA);
}
test11a();

proc inoutEarlyReturn1(inout arg: R) {
  return;
  arg = makeR();
}

proc test12() {
  writeln("test12");
  var x: R;
  inoutEarlyReturn1(x);
  writeln(x);
}
test12();

proc inoutEarlyReturn2(inout arg: R) {
  if option then return;
  arg = makeR();
}

proc test13() {
  writeln("test13");
  var x: R;
  inoutEarlyReturn2(x);
  writeln(x);
}
test13();

proc inoutEarlyReturn3(inout arg: R) {
  if !option then return;
  arg = makeR();
}

proc test14() {
  writeln("test14");
  var x: R;
  inoutEarlyReturn3(x);
  writeln(x);
}
test14();

proc inoutEarlyReturn4(inout arg: R) {
  if paramOption then return;
  arg = makeR();
}

proc test15() {
  writeln("test15");
  var x: R;
  inoutEarlyReturn4(x);
  writeln(x);
}
test15();

proc inoutEarlyReturn5(inout arg: R) {
  if !paramOption then return;
  arg = makeR();
}

proc test16() {
  writeln("test16");
  var x: R;
  inoutEarlyReturn5(x);
  writeln(x);
}
test16();

proc inoutCond1(inout arg: R) {
  if option {
    arg = makeR(1);
  } else {
    arg = makeR(2);
  }
}

proc test17() {
  writeln("test17");
  var x: R;
  inoutCond1(x);
  writeln(x);
}
test17();

proc inoutCond2(inout arg: R) {
  if !option {
    arg = makeR(1);
  } else {
    arg = makeR(2);
  }
}

proc test18() {
  writeln("test18");
  var x: R;
  inoutCond2(x);
  writeln(x);
}
test18();

proc inoutCond3(inout arg: R) {
  if option {
    arg = makeR(1);
  }
}

proc test19() {
  writeln("test19");
  var x: R;
  inoutCond3(x);
  writeln(x);
}
test19();

proc inoutCond4(inout arg: R) {
  if !option {
    arg = makeR(1);
  }
}

proc test20() {
  writeln("test20");
  var x: R;
  inoutCond4(x);
  writeln(x);
}
test20();

proc inoutCond1p(inout arg: R) {
  if paramOption {
    arg = makeR(1);
  } else {
    arg = makeR(2);
  }
}

proc test17p() {
  writeln("test17p");
  var x: R;
  inoutCond1p(x);
  writeln(x);
}
test17p();

proc inoutCond2p(inout arg: R) {
  if !paramOption {
    arg = makeR(1);
  } else {
    arg = makeR(2);
  }
}

proc test18p() {
  writeln("test18p");
  var x: R;
  inoutCond2p(x);
  writeln(x);
}
test18p();

proc inoutCond3p(inout arg: R) {
  if paramOption {
    arg = makeR(1);
  }
}

proc test19p() {
  writeln("test19p");
  var x: R;
  inoutCond3p(x);
  writeln(x);
}
test19p();

proc inoutCond4p(inout arg: R) {
  if !paramOption {
    arg = makeR(1);
  }
}

proc test20p() {
  writeln("test20p");
  var x: R;
  inoutCond4p(x);
  writeln(x);
}
test20p();

proc inoutThrows1(inout arg: R) throws {
}

proc test21() throws {
  writeln("test21");
  var x: R;
  inoutThrows1(x);
  writeln(x);
}
try { test21(); } catch e { writeln(e); }

proc test21a() {
  writeln("test21a");
  var x: R;
  try! inoutThrows1(x);
  writeln(x);
}
test21a();

proc inoutThrows2(inout arg: R) throws {
  throw new Error();
}

proc test22() throws {
  writeln("test22");
  var x: R;
  inoutThrows2(x);
  writeln(x);
}
try { test22(); } catch e { writeln(e); }

proc inoutThrows3(inout arg: R) throws {
  arg = makeR(1);
}

proc test23() throws {
  writeln("test23");
  var x: R;
  inoutThrows3(x);
  writeln(x);
}
try { test23(); } catch e { writeln(e); }

proc test23a() {
  writeln("test23a");
  var x: R;
  try! inoutThrows3(x);
  writeln(x);
}
test23a();


proc inoutThrows4(inout arg: R) throws {
  throw new Error();
  arg = makeR(1);
}

proc test24() throws {
  writeln("test24");
  var x: R;
  inoutThrows4(x);
  writeln(x);
}
try { test24(); } catch e { writeln(e); }

proc inoutThrows5(inout arg: R) throws {
  if option then throw new Error();
  arg = makeR(1);
}

proc test25() throws {
  writeln("test25");
  var x: R;
  inoutThrows5(x);
  writeln(x);
}
try { test25(); } catch e { writeln(e); }

proc inoutThrows6(inout arg: R) throws {
  if !option then throw new Error();
  arg = makeR(1);
}

proc test26() throws {
  writeln("test26");
  var x: R;
  inoutThrows6(x);
  writeln(x);
}
try { test26(); } catch e { writeln(e); }
