record R {
  var x:int = 0;
  proc init() {
    writeln("init");
  }
  proc deinit() {
    assert(x == 0);
    x = 99;
    writeln("deinit");
  }
}

config const option = true;

proc t22() {
  writeln("t22");
  var r;
  if option then
    return;
  r = new R();
  writeln("end outer");
}
t22();

proc t23() {
  writeln("t23");
  var r;
  if !option then
    return;
  r = new R();
  writeln("end outer");
}
t23();

proc t24() {
  writeln("t24");
  var r;
  {
    if option then
      return;
    {
      r = new R();
    }
  }
  writeln("end outer");
}
t24();

proc t25() {
  writeln("t25");
  var r;
  {
    if !option then
      return;
    {
      r = new R();
    }
  }
  writeln("end outer");
}
t25();

proc t26() {
  writeln("t26");
  var r;
  {
    if option {
      if option then return;
      r = new R();
    } else {
      r = new R();
    }
  }
  writeln("end outer");
}
t26();

proc t26a() {
  writeln("t26a");
  var r;
  {
    if !option {
      r = new R();
    } else {
      if option then return;
      r = new R();
    }
  }
  writeln("end outer");
}
t26a();


proc t27() {
  writeln("t27");
  var r;
  {
    if option {
      r = new R();
      if option then return;
    } else {
      r = new R();
    }
  }
  writeln("end outer");
}
t27();

proc t27a() {
  writeln("t27a");
  var r;
  {
    if !option {
      r = new R();
    } else {
      r = new R();
      if option then return;
    }
  }
  writeln("end outer");
}
t27a();


proc t28() {
  writeln("t28");
  var r;
  {
    if option {
      r = new R();
    } else {
      r = new R();
    }
    if option then return;
  }
  writeln("end outer");
}
t28();

proc t29() {
  writeln("t29");
  var r;
  {
    if option {
      r = new R();
    } else {
      r = new R();
    }
  }
  if option then return;
  writeln("end outer");
}
t29();

proc maybeThrow() throws {
  if option then throw new Error();
}
proc t30() throws {
  writeln("t30");
  var r;
  maybeThrow();
  r = new R();
  writeln("end outer");
}
try {
  t30();
} catch e {
  writeln(e);
}

proc t31() throws {
  writeln("t31");
  var r;
  {
    maybeThrow();
    r = new R();
  }
  writeln("end outer");
}
try {
  t31();
} catch e {
  writeln(e);
}

