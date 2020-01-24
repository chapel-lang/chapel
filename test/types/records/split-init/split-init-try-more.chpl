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

proc makeR(arg: int, error: bool) throws {
  if error then
    throw new Error();
  
  return new R(arg);
}

proc test1() {
  writeln("test1");
  var r:R;
  
  try {
    r = makeR(1, false);
  } catch {
    writeln("error encountered");
    return;
  }
}
test1();

proc test2() {
  writeln("test2");
  var r:R;
  
  try {
    r = makeR(1, true);
  } catch {
    writeln("error encountered");
    return;
  }
}
test2();

proc testAB00() {
  writeln("testAB00");
  var a:R;
  var b:R;
  
  try {
    a = makeR(1, false);
    b = makeR(2, false);
  } catch {
    writeln("error encountered");
    return;
  }
}
testAB00();

proc testAB01() {
  writeln("testAB01");
  var a:R;
  var b:R;
  
  try {
    a = makeR(1, false);
    b = makeR(2, true);
  } catch {
    writeln("error encountered");
    return;
  }
}
testAB01();

proc testAB10() {
  writeln("testAB10");
  var a:R;
  var b:R;
  
  try {
    a = makeR(1, true);
    b = makeR(2, false);
  } catch {
    writeln("error encountered");
    return;
  }
}
testAB10();

proc testAB11() {
  writeln("testAB11");
  var a:R;
  var b:R;
  
  try {
    a = makeR(1, true);
    b = makeR(2, true);
  } catch {
    writeln("error encountered");
    return;
  }
}
testAB11();
