config const option = true;

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
  var outer = makeR(0, false);
  var a:R;
  var b:R;

  try {
    a = makeR(1, false);
    b = makeR(2, false);
  } catch {
    writeln("error encountered");
    return;
  }

  writeln(outer, " ", a, " ", b);
  writeln("end");
}
testAB00();

proc testAB01() {
  writeln("testAB01");
  var outer = makeR(0, false);
  var a:R;
  var b:R;

  try {
    a = makeR(1, false);
    b = makeR(2, true);
  } catch {
    writeln("error encountered");
    return;
  }

  writeln(outer, " ", a, " ", b);
  writeln("end");
}
testAB01();

proc testAB10() {
  writeln("testAB10");
  var outer = makeR(0, false);
  var a:R;
  var b:R;

  try {
    a = makeR(1, true);
    b = makeR(2, false);
  } catch {
    writeln("error encountered");
    return;
  }

  writeln(outer, " ", a, " ", b);
  writeln("end");
}
testAB10();

proc testAB11() {
  writeln("testAB11");
  var outer = makeR(0, false);
  var a:R;
  var b:R;

  try {
    a = makeR(1, true);
    b = makeR(2, true);
  } catch {
    writeln("error encountered");
    return;
  }

  writeln(outer, " ", a, " ", b);
  writeln("end");
}
testAB11();

proc test1eob() {
  writeln("test1eob");
  var r:R;

  try {
    r = makeR(1, false);
  } catch {
    writeln("error encountered");
    return;
  }

  ref rr = r;
}
test1eob();

proc test2eob() {
  writeln("test2eob");
  var r:R;

  try {
    r = makeR(1, true);
  } catch {
    writeln("error encountered");
    return;
  }

  r;
}
test2eob();

proc testAB00eob() {
  writeln("testAB00eob");
  var a:R;
  var b:R;

  try {
    a = makeR(1, false);
    b = makeR(2, false);
  } catch {
    writeln("error encountered");
    return;
  }

  ref ra = a;
  ref rb = b;
}
testAB00eob();

proc testAB01eob() {
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

  ref ra = a;
  ref rb = b;
}
testAB01eob();

proc testAB10eob() {
  writeln("testAB10eob");
  var a:R;
  var b:R;

  try {
    a = makeR(1, true);
    b = makeR(2, false);
  } catch {
    writeln("error encountered");
    return;
  }

  a; b;
}
testAB10eob();

proc testAB11eob() {
  writeln("testAB11eob");
  var a:R;
  var b:R;

  try {
    a = makeR(1, true);
    b = makeR(2, true);
  } catch {
    writeln("error encountered");
    return;
  }

  ref ra = a;
  ref rb = b;
}
testAB11eob();


proc makeRNoThrow(arg: int) {
  return new R(arg);
}

proc doThrow() throws {
  throw new Error();
}
proc doThrow(arg) throws {
  throw new Error();
}

proc f(a, b) { }

proc test11() {
  writeln("test11");
  var r:R;

  try {
    r = new R(1);
    doThrow(r);
    doThrow();
  } catch {
    writeln("error encountered");
    return;
  }
}
test11();

proc test12() {
  writeln("test12");

  try {
    var r:R;
    r = new R(1);
    doThrow(r);
    doThrow();
  } catch {
    writeln("error encountered");
    return;
  }
}
test12();

proc test13() {
  writeln("test13");

  try {
    var r:R;
    r = new R(1);
    ref rr = r;
    doThrow(r);
    doThrow();
  } catch {
    writeln("error encountered");
    return;
  }
}
test13();

proc test14() {
  writeln("test14");

  try {
    var r:R;
    r = makeR(1, true);
    doThrow(r);
    doThrow();
  } catch {
    writeln("error encountered");
    return;
  }
}
test14();

proc test15() {
  writeln("test15");

  try {
    var r:R;
    r = makeR(1, true);
    ref rr = r;
    doThrow(r);
    doThrow();
  } catch {
    writeln("error encountered");
    return;
  }
}
test15();

proc test16() {
  writeln("test16");

  try {
    makeR(1, true);
    doThrow();
  } catch {
    writeln("error encountered");
    return;
  }
}
test16();

proc test611() {
  writeln("test611");

  try {
    f(makeR(1, true), makeR(2, true));
    writeln("after f");
  } catch {
    writeln("error encountered");
    return;
  }
}
test611();

proc test610() {
  writeln("test610");

  try {
    f(makeR(1, true), makeR(2, false));
    writeln("after f");
  } catch {
    writeln("error encountered");
    return;
  }
}
test610();

proc test601() {
  writeln("test601");

  try {
    f(makeR(1, false), makeR(2, true));
    writeln("after f");
  } catch {
    writeln("error encountered");
    return;
  }
}
test601();

proc test600() {
  writeln("test600");

  try {
    f(makeR(1, false), makeR(2, false));
    writeln("after f");
  } catch {
    writeln("error encountered");
    return;
  }
}
test600();


proc test17() {
  writeln("test17");

  try {
    makeRNoThrow(1);
    if option {
      doThrow();
    } else {
    }
  } catch {
    writeln("error encountered");
    return;
  }
}
test17();

proc test2000() {
  writeln("test2000");

  var outer = makeR(0, false);
  var r;

  try {
    writeln("begin try");
    if option {
      r = makeR(1, false);
    } else {
      r = makeR(2, false);
    }
    writeln("end try");
  } catch e {
    writeln("error encountered");
    return;
  }
}
proc test2001() {
  writeln("test2010");

  var outer = makeR(0, false);
  var r;

  try {
    writeln("begin try");
    if option {
      r = makeR(1, true);
    } else {
      r = makeR(2, false);
    }
    writeln("end try");
  } catch e {
    writeln("error encountered");
    return;
  }

  outer;
}
test2001();

proc test2100() {
  writeln("test2100");

  var outer = makeR(0, false);
  var r;

  try {
    writeln("begin try");
    if !option {
      r = makeR(1, false);
    } else {
      r = makeR(2, false);
    }
    writeln("end try");
  } catch e {
    writeln("error encountered");
    return;
  }

  outer;
}
test2100();

proc test2101() {
  writeln("test2101");

  var outer = makeR(0, false);
  var r;

  try {
    writeln("begin try");
    if !option {
      r = makeR(1, false);
    } else {
      r = makeR(2, true);
    }
    writeln("end try");
  } catch e {
    writeln("error encountered");
    return;
  }

  outer;
}
test2101();

proc test3000() {
  writeln("test3000");

  var outer = makeR(0, false);
  var r;

  if option {
    try {
      writeln("begin try1");
      r = makeR(1, false);
      writeln("end try1");
    } catch e {
      writeln("error encountered1");
      return;
    }
  } else {
    try {
      writeln("begin try2");
      r = makeR(2, false);
      writeln("end try2");
    } catch e {
      writeln("error encountered2");
      return;
    }
  }

  outer;
}
test3000();

proc test3010() {
  writeln("test3010");

  var outer = makeR(0, false);
  var r;

  if option {
    try {
      writeln("begin try1");
      r = makeR(1, true);
      writeln("end try1");
    } catch e {
      writeln("error encountered1");
      return;
    }
  } else {
    try {
      writeln("begin try2");
      r = makeR(2, false);
      writeln("end try2");
    } catch e {
      writeln("error encountered2");
      return;
    }
  }

  outer;
}
test3010();

proc test3111() {
  writeln("test3111");

  var outer = makeR(0, false);
  var r;

  if !option {
    try {
      writeln("begin try1");
      r = makeR(1, true);
      writeln("end try1");
    } catch e {
      writeln("error encountered1");
      return;
    }
  } else {
    try {
      writeln("begin try2");
      r = makeR(2, true);
      writeln("end try2");
    } catch e {
      writeln("error encountered2");
      return;
    }
  }

  outer;
}
test3111();

proc test19() throws {
  writeln("test19");

  var r: R;

  {
    writeln("begin inner");
    try {

      {
        r = makeR(1, false);
        doThrow();
      }
    } catch e {
      writeln("error encountered");
      throw e;
    }
    writeln("end inner");
  }
}
try { test19(); } catch e { writeln(e); }


proc test20() throws {
  writeln("test20");

  var a: R;
  var b: R;
  var c: R;

  {
    writeln("begin inner");
    try {

      {
        a = makeR(1, false);
        b = makeR(2, false);
        c = makeR(3, false);
      }
    } catch e {
      writeln("error encountered");
      throw e;
    }
    writeln("end inner");
  }
}
try { test20(); } catch e { writeln(e); }

proc test21() throws {
  writeln("test21");

  var a: R;
  var b: R;
  var c: R;

  {
    writeln("begin inner");
    try {

      {
        a = makeR(1, false);
        b = makeR(2, false);
        c = makeR(3, false);
        doThrow();
      }
    } catch e {
      writeln("error encountered");
      throw e;
    }
    writeln("end inner");
  }
}
try { test21(); } catch e { writeln(e); }

proc test22() throws {
  writeln("test22");

  var a: R;
  var b: R;
  var c: R;

  {
    writeln("begin inner");
    try {

      {
        a = makeR(1, false);
        b = makeR(2, false);
        c = makeR(3, false);
        doThrow();
      }
    } catch e {
      writeln("error encountered");
    }
    writeln("end inner");
  }

  writeln(a, " ", b, " ", c);
}
try { test22(); } catch e { writeln(e); }
