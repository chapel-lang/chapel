use LowLevelMove;

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
    writeln("deinit ", this.x);
  }
}
proc =(ref lhs:R, rhs:R) {
  writeln("= ", lhs.x, " ", rhs.x);
  lhs.x = rhs.x;
}

proc test1a() {
  writeln("test1a");
  var D = {"one", "two"};
  var A:[D] R = noinit;
}
test1a();

proc test1b() {
  writeln("test1b");
  var D = {"one", "two"};
  var A:[D] R = noinit;

  for (i, j) in zip(D, 1..) {
    moveInitialize(A[i], new R(j));
  }

  A.dsiElementInitializationComplete();
}
test1b();

proc test1c() {
  writeln("test1c");
  var D = {"one", "two"};
  var A:[D] R = noinit;

  moveInitialize(A["two"], new R(2));
  explicitDeinit(A["two"]);
}
test1c();



class FieldNoinitDefaultInitializer {
  var D = {"one", "two"};
  var A:[D] R = noinit;
}

proc test2a() {
  writeln("test2a");
  var c = new FieldNoinitDefaultInitializer();
}
test2a();

proc test2b() {
  writeln("test2b");
  var c = new FieldNoinitDefaultInitializer();

  for (i, j) in zip(c.D, 1..) {
    moveInitialize(c.A[i], new R(j));
  }

  c.A.dsiElementInitializationComplete();
}
test2b();

proc test2c() {
  writeln("test2c");
  var c = new FieldNoinitDefaultInitializer();

  moveInitialize(c.A["two"], new R(2));
  explicitDeinit(c.A["two"]);
}
test2c();



class FieldNoinitUserInitializer {
  var D = {"one", "two"};
  var A:[D] R;
  proc init() {
    this.A = noinit;
  }
}

proc test3a() {
  writeln("test3a");
  var c = new FieldNoinitUserInitializer();
}
test3a();

proc test3b() {
  writeln("test3b");
  var c = new FieldNoinitUserInitializer();

  for (i, j) in zip(c.D, 1..) {
    moveInitialize(c.A[i], new R(j));
  }

  c.A.dsiElementInitializationComplete();
}
test3b();

proc test3c() {
  writeln("test3c");
  var c = new FieldNoinitUserInitializer();

  moveInitialize(c.A["two"], new R(2));
  explicitDeinit(c.A["two"]);
}
test3c();
