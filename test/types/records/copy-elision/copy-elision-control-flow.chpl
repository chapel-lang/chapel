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

proc returnsBool(arg) {
  return true;
}

proc acceptIn(in arg: R) {
}

proc acceptInAndReturn(in arg: R) {
  return arg;
}

config const option = true;

proc test1a() {
  writeln("test1a");
  var x = new R(1);
  var y = x;
  writeln(y.toString());
}
test1a();

proc test1b() {
  writeln("test1b");
  var x = new R(1);
  var y;
  y = x;
  writeln(y.toString());
}
test1b();

proc test2() {
  writeln("test2");
  var x = new R(1);
  {
    var y = x;
  }
}
test2();

proc test3() {
  writeln("test3");
  var x = new R(1);
  {
    var y = x;
    return;
  }
}
test3();

proc test4a() {
  writeln("test4a");
  var x = new R(1);
  for i in 1..1 {
    var y = x;
  }
}
test4a();

proc test4b() {
  writeln("test4b");
  var x = new R(1);
  coforall i in 1..1 {
    var y = x;
  }
}
test4b();

proc test4c() {
  writeln("test4c");
  var x = new R(1);
  forall i in 1..1 {
    var y = x;
  }
}
test4c();

proc test4d() {
  writeln("test4d");
  var x = new R(1);
  do {
    var y = x;
  } while (false);
}
test4d();

proc test4e() {
  writeln("test4e");
  var x = new R(1);
  var go = true;
  while (go) {
    var y = x;
    go = false;
  }
}
test4e();

proc test5a() {
  writeln("test5a");
  var x = new R(1);
  on Locales[numLocales-1] {
    var y = x;
  }
}
test5a();

proc test5b() {
  writeln("test5b");
  var x = new R(1);
  cobegin {
    {
      var y = x;
    }
    {
      // do nothing
    }
  }
}
test5b();

proc test5c() {
  writeln("test5c");
  var x = new R(1);
  sync {
    begin {
      var y = x;
    }
  }
}
test5c();

proc test5d() {
  writeln("test5d");
  var x = new R(1);
  var done: sync int;
  begin {
    var y = x;
    done = 1;
  }

  done; // wait for task
}
test5d();

proc test6a() {
  writeln("test6a");
  var x = new R(1);
  if option {
    var y = x;
  } else {
    var y = x;
  }
}
test6a();

proc test6b() {
  writeln("test6b");
  var x = new R(1);
  var y:R;
  if option {
    y = x;
  } else {
    y = x;
  }
}
test6b();

proc test7a() {
  writeln("test7a");
  var x = new R(1);
  try! {
    var y = x;
  }
}
test7a();

proc test7b() {
  writeln("test7b");
  var x = new R(1);
  var y;
  try! {
    y = x;
  }
}
test7b();

proc test7c() throws {
  writeln("test7c");
  var x = new R(1);
  try {
    var y = x;
  }
}
try! test7c();

proc test7d() throws {
  writeln("test7d");
  var x = new R(1);
  var y;
  try {
    y = x;
  }
}
try! test7d();

proc test8a() throws {
  writeln("test8a");
  var x = new R(1);
  var y: R;

  if option then
    throw new Error();

  y = x;
}
try { test8a(); } catch e { writeln(e); }

proc test8b() throws {
  writeln("test8b");
  var x = new R(1);
  var y: R;

  y = x;
  
  if option then
    throw new Error();
}
try { test8b(); } catch e { writeln(e); }

proc test8c() throws {
  writeln("test8c");
  var x = new R(1);
  var y: R;

  if !option then
    throw new Error();

  y = x;
}
try { test8c(); } catch e { writeln(e); }

proc test8d() throws {
  writeln("test8d");
  var x = new R(1);
  var y: R;

  y = x;
  if !option then
    throw new Error();
}
try { test8d(); } catch e { writeln(e); }

proc test9a() {
  writeln("test9a");
  var x = new R(1);
  var y: R;

  if option then
    return;

  y = x;
}
test9a();

proc test9b() {
  writeln("test9b");
  var x = new R(1);
  var y: R;

  y = x;
  
  if option then
    return;
}
test9b();

proc test9c() {
  writeln("test9c");
  var x = new R(1);
  var y: R;

  if !option then
    return;

  y = x;
}
test9c();

proc test9d() {
  writeln("test9d");
  var x = new R(1);
  var y: R;

  y = x;
  if !option then
    return;
}
test9d();

proc test11() {
  writeln("test11");
  var x = new R(1);
  return acceptInAndReturn(x);
}
test11();

proc test12() {
  writeln("test12");
  var x = new R(1);

  var y = x;

  if returnsBool(x) {
  } else {
  }
}
test12();

proc test13a() {
  writeln("test13a");
  var x = new R(1);
  var y;

  {
    if option {
      return;
    } else {
      y = x;
    }
  }
}
test13a();

proc test13b() {
  writeln("test13b");
  var x = new R(1);
  var y;

  if option {
    y = x;
  } else {
    return;
  }
}
test13b();

proc test13c() throws {
  writeln("test13c");
  var x = new R(1);
  var y;

  if option {
    throw new Error();
  } else {
    y = x;
  }
}
try { test13c(); } catch e { writeln(e); }

proc test13d() throws {
  writeln("test13d");
  var x = new R(1);
  {
    if option {
      var y = x;
    } else {
      throw new Error();
    }
  }
}
try { test13d(); } catch e { writeln(e); }

proc test14a() {
  writeln("test14a");
  var x = new R(1);

  if option {
    var y = x;
    return;
  }
}
test14a();

proc test14b() {
  writeln("test14b");
  var x = new R(1);

  if !option {
    var y = x;
    return;
  }
}
test14b();

proc test14c() throws {
  writeln("test14c");
  var x = new R(1);

  if option {
    var y = x;
    throw new Error();
  }
}
try { test14c(); } catch e { writeln(e); }

proc test14d() throws {
  writeln("test14d");
  var x = new R(1);

  if option {
    var y = x;
    throw new Error();
  }
}
try { test14d(); } catch e { writeln(e); }

proc test15() {
  writeln("test15");
  var x = new R(1);

  if option {
    var y;
    if option {
      y = x;
    } else {
      y = x;
    }
    return;
  }
}
test15();

proc test16a() {
  writeln("test16a");
  var x = new R(1);
  if option {
    var y = x;
  } else {
  }
}
test16a();

proc test16b() {
  writeln("test16b");
  var x = new R(1);
  if option {
  } else {
    var y = x;
  }
}
test16b();

proc test16c() {
  writeln("test16c");
  var x = new R(1);
  if !option {
    var y = x;
  } else {
  }
}
test16c();

proc test16d() {
  writeln("test16d");
  var x = new R(1);
  if !option {
  } else {
    var y = x;
  }
}
test16d();
