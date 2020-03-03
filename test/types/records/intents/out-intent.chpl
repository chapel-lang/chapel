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

proc out1a(out arg: R) {
  writeln(arg);
  arg = makeR();
}

proc test1a() {
  writeln("test1a");
  var x: R;
  out1a(x);
  writeln(x);
}
test1a();

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

proc outGenericArray1(out arr) {
  writeln(arr.domain);
}

proc test6() {
  writeln("test6");
  var AA:[1..1] R;
  outGenericArray1(AA);
  writeln(AA);
}
test6();

proc test6a() {
  writeln("test6a");
  var AA:[1..1] R;
  writeln(AA.domain, " ", AA);
  outGenericArray1(AA);
  writeln(AA);
}
test6a();

proc outGenericArray2(out arr) {
  var B:[1..1] R;
  B[1] = makeR(1);
  arr = B;
}

proc test7() {
  writeln("test7");
  var AA:[1..1] R;
  outGenericArray2(AA);
  writeln(AA);
}
test7();

proc outGenericArray2a(out arr) {
  writeln(arr.domain, " ", arr); // preventing split-init
  var B:[1..1] R;
  B[1] = makeR(1);
  arr = B;
}

proc test7a() {
  writeln("test7a");
  var AA:[1..1] R;
  outGenericArray2a(AA);
  writeln(AA);
}
test7a();

proc outPartialArray1(out arr:[] R) {
}

proc test8() {
  writeln("test8");
  var AA:[1..1] R;
  outPartialArray1(AA);
  writeln(AA);
}
test8();

proc outPartialArray2(out arr:[] R) {
  var B:[1..1] R;
  B[1] = makeR(1);
  arr = B;
}

proc test9() {
  writeln("test9");
  var AA:[1..1] R;
  outPartialArray2(AA);
  writeln(AA);
}
test9();


proc outConcreteArray1(out arr:[1..1] R) {
}

proc test10() {
  writeln("test10");
  var AA:[1..1] R;
  outConcreteArray1(AA);
  writeln(AA);
}
test10();

proc test10b() {
  writeln("test10b");
  var AA:[1..1] R;
  writeln(AA); // preventing split init
  outConcreteArray1(AA);
  writeln(AA);
}
test10b();

proc outConcreteArray2(out arr:[1..1] R) {
  var B:[1..1] R;
  B[1] = makeR(1);
  arr = B;
}

proc test11() {
  writeln("test11");
  var AA:[1..1] R;
  outConcreteArray2(AA);
  writeln(AA);
}
test11();

proc outConcreteArray2a(out arr:[1..1] R) {
  writeln(arr.domain, " ", arr); // preventing split init
  var B:[1..1] R;
  B[1] = makeR(1);
  arr = B;
}

proc test11a() {
  writeln("test11a");
  var AA:[1..1] R;
  outConcreteArray2a(AA);
  writeln(AA);
}
test11a();

proc outEarlyReturn1(out arg: R) {
  return;
  arg = makeR();
}

proc test12() {
  writeln("test12");
  var x: R;
  outEarlyReturn1(x);
  writeln(x);
}
test12();

proc outEarlyReturn2(out arg: R) {
  if option then return;
  arg = makeR();
}

proc test13() {
  writeln("test13");
  var x: R;
  outEarlyReturn2(x);
  writeln(x);
}
test13();

proc outEarlyReturn3(out arg: R) {
  if !option then return;
  arg = makeR();
}

proc test14() {
  writeln("test14");
  var x: R;
  outEarlyReturn3(x);
  writeln(x);
}
test14();

proc outEarlyReturn4(out arg: R) {
  if paramOption then return;
  arg = makeR();
}

proc test15() {
  writeln("test15");
  var x: R;
  outEarlyReturn4(x);
  writeln(x);
}
test15();

proc outEarlyReturn5(out arg: R) {
  if !paramOption then return;
  arg = makeR();
}

proc test16() {
  writeln("test16");
  var x: R;
  outEarlyReturn5(x);
  writeln(x);
}
test16();

proc outCond1(out arg: R) {
  if option {
    arg = makeR(1);
  } else {
    arg = makeR(2);
  }
}

proc test17() {
  writeln("test17");
  var x: R;
  outCond1(x);
  writeln(x);
}
test17();

proc outCond2(out arg: R) {
  if !option {
    arg = makeR(1);
  } else {
    arg = makeR(2);
  }
}

proc test18() {
  writeln("test18");
  var x: R;
  outCond2(x);
  writeln(x);
}
test18();

proc outCond3(out arg: R) {
  if option {
    arg = makeR(1);
  }
}

proc test19() {
  writeln("test19");
  var x: R;
  outCond3(x);
  writeln(x);
}
test19();

proc outCond4(out arg: R) {
  if !option {
    arg = makeR(1);
  }
}

proc test20() {
  writeln("test20");
  var x: R;
  outCond4(x);
  writeln(x);
}
test20();

proc outCond1p(out arg: R) {
  if paramOption {
    arg = makeR(1);
  } else {
    arg = makeR(2);
  }
}

proc test17p() {
  writeln("test17p");
  var x: R;
  outCond1p(x);
  writeln(x);
}
test17p();

proc outCond2p(out arg: R) {
  if !paramOption {
    arg = makeR(1);
  } else {
    arg = makeR(2);
  }
}

proc test18p() {
  writeln("test18p");
  var x: R;
  outCond2p(x);
  writeln(x);
}
test18p();

proc outCond3p(out arg: R) {
  if paramOption {
    arg = makeR(1);
  }
}

proc test19p() {
  writeln("test19p");
  var x: R;
  outCond3p(x);
  writeln(x);
}
test19p();

proc outCond4p(out arg: R) {
  if !paramOption {
    arg = makeR(1);
  }
}

proc test20p() {
  writeln("test20p");
  var x: R;
  outCond4p(x);
  writeln(x);
}
test20p();

proc outThrows1(out arg: R) throws {
}

proc test21() throws {
  writeln("test21");
  var x: R;
  outThrows1(x);
  writeln(x);
}
try { test21(); } catch e { writeln(e); }

proc test21a() {
  writeln("test21a");
  var x: R;
  try! outThrows1(x);
  writeln(x);
}
test21a();

proc outThrows2(out arg: R) throws {
  throw new Error();
}

proc test22() throws {
  writeln("test22");
  var x: R;
  outThrows2(x);
  writeln(x);
}
try { test22(); } catch e { writeln(e); }

proc outThrows3(out arg: R) throws {
  arg = makeR(1);
}

proc test23() throws {
  writeln("test23");
  var x: R;
  outThrows3(x);
  writeln(x);
}
try { test23(); } catch e { writeln(e); }

proc test23a() {
  writeln("test23a");
  var x: R;
  try! outThrows3(x);
  writeln(x);
}
test23a();


proc outThrows4(out arg: R) throws {
  throw new Error();
  arg = makeR(1);
}

proc test24() throws {
  writeln("test24");
  var x: R;
  outThrows4(x);
  writeln(x);
}
try { test24(); } catch e { writeln(e); }

proc outThrows5(out arg: R) throws {
  if option then throw new Error();
  arg = makeR(1);
}

proc test25() throws {
  writeln("test25");
  var x: R;
  outThrows5(x);
  writeln(x);
}
try { test25(); } catch e { writeln(e); }

proc outThrows6(out arg: R) throws {
  if !option then throw new Error();
  arg = makeR(1);
}

proc test26() throws {
  writeln("test26");
  var x: R;
  outThrows6(x);
  writeln(x);
}
try { test26(); } catch e { writeln(e); }
