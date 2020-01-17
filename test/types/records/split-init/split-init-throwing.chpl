config const cond = false;

class C { }
record R {
  var x: int = 0;
  var ptr: owned C = new owned C();
  proc init() {
    this.x = 0;
    writeln("init (default)");
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

config const case = 0;

proc makeR(arg:int) throws {
  if case == 1 then
    throw new Error();

  return new R(arg);
}

proc test1() throws {
  writeln("test1");

  var x;
  if case == 2 then
    throw new Error();
  x = makeR(100);

  if case == 3 then
    throw new Error();

  writeln("end test1");
}

try {
  test1();
} catch e {
  writeln(e);
}

proc test2() throws {
  writeln("test2");

  var x;
  if case == 2 then
    throw new Error();
  x = makeR(100);
  ref rx = x;

  if case == 3 then
    throw new Error();

  writeln("end test2");
}

try {
  test2();
} catch e {
  writeln(e);
}
