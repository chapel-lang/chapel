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
  var D = {1..7};
  var A:[D] R = noinit;

  // Expecting that this resize does not cause the
  // new elements to be default initialized.
  D = {1..10};

  for i in D {
    moveInitialize(A[i], new R(i));
  }
  A.dsiElementInitializationComplete();
}
test1a();
