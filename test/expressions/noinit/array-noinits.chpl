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
  var A:[1..2] R = noinit;
}
test1a();

proc test1b() {
  writeln("test1b");
  var A:[1..2] R = noinit;

  for i in 1..2 {
    moveInitialize(A[i], new R(i));
  }

  A.dsiElementInitializationComplete();
}
test1b();

proc test1c() {
  writeln("test1c");
  var A:[1..2] R = noinit;

  moveInitialize(A[2], new R(2));
  explicitDeinit(A[2]);
}
test1c();



class FieldNoinitDefaultInitializer {
  var A:[1..2] R = noinit;
}

proc test2a() {
  writeln("test2a");
  var c = new FieldNoinitDefaultInitializer();
}
test2a();

proc test2b() {
  writeln("test2b");
  var c = new FieldNoinitDefaultInitializer();

  for i in 1..2 {
    moveInitialize(c.A[i], new R(i));
  }

  c.A.dsiElementInitializationComplete();
}
test2b();

proc test2c() {
  writeln("test2c");
  var c = new FieldNoinitDefaultInitializer();

  moveInitialize(c.A[2], new R(2));
  explicitDeinit(c.A[2]);
}
test2c();



class FieldNoinitUserInitializer {
  var A:[1..2] R;
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

  for i in 1..2 {
    moveInitialize(c.A[i], new R(i));
  }

  c.A.dsiElementInitializationComplete();
}
test3b();

proc test3c() {
  writeln("test3c");
  var c = new FieldNoinitUserInitializer();

  moveInitialize(c.A[2], new R(2));
  explicitDeinit(c.A[2]);
}
test3c();
