config const cond = false;

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

// Just indicates other statements
proc f(arg) { }

proc makeR(arg:int) throws {
  return new R(arg);
}

proc test() throws {
  writeln("test");

  {
    writeln("x");
    var x:R = makeR(0);
  }

  {
    writeln("yes0");
    var yes0:R;
    try {
      yes0 = makeR(0);
    }
    writeln(yes0);
  }
 
  {
    writeln("yes1");
    var yes1:R;
    try {
      yes1 = makeR(1);
    } catch {
      return;
    }
    writeln(yes1);
  }

  {
    writeln("yes2");
    var yes2:R;
    try {
      yes2 = makeR(2);
    } catch {
      throw new Error();
    }
    writeln(yes2);
  }

  {
    writeln("yes3");
    var yes3:R;
    try {
      yes3 = makeR(3);
    } catch {
      throw new Error();
    }
    writeln(yes3);
  }

  {
    writeln("yes1b");
    var yes1b:R;
    try! {
      yes1b = makeR(1);
    } catch {
      return;
    }
    writeln(yes1b);
  }

  {
    writeln("yes2b");
    var yes2b:R;
    try! {
      yes2b = makeR(2);
    } catch {
      throw new Error();
    }
    writeln(yes2b);
  }

  {
    writeln("yes3b");
    var yes3b:R;
    try {
      yes3b = makeR(3);
    } catch {
      throw new Error();
    }
    writeln(yes3b);
  }
}
try! test();

proc test1() throws {
  writeln("no1");
  var no1:R;
  try {
    no1 = makeR(1);
  } catch e {
    writeln(e);
  }
  writeln(no1);
}
try! test1();

proc test2() throws {
  writeln("no2");
  var no2:R;
  try {
    return;
    no2 = makeR(1);
  } catch e {
    writeln(e);
  }
  writeln(no2);
}
try! test2();
 
proc test3() throws {
  writeln("no3");
  var no3:R;
  try {
  } catch e {
    no3 = makeR(1);
    writeln(e);
  }
  writeln(no3);
}
try! test3();

proc test4() throws {
  writeln("no4");
  var no4:R;
  try {
    return;
  } catch e {
    no4 = makeR(1);
    writeln(e);
  }
  writeln(no4);
}
try! test4();

proc test5() throws {
  writeln("no5");
  var no5:R;
  try {
    no5 = makeR(1);
    throw new Error();
  } catch e {
    no5 = makeR(2);
  }
  writeln(no5);
}
try! test5();

proc test6() throws {
  writeln("no6");
  var no6:R;
  try {
    no6 = makeR(1);
    throw new Error();
  } catch e {
    no6 = makeR(2);
  }
  writeln(no6);
}
try! test6();
