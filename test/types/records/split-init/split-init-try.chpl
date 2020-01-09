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
}
proc =(ref lhs:R, rhs:R) {
  writeln("= ", lhs.x, " ", rhs.x);
}

// Just indicates other statements
proc f(arg) { }

proc makeR(arg:int) throws {
  return new R(arg);
}

proc test() throws {
  writeln("test");

  writeln("x");
  var x:R = makeR(0);

  writeln("yes1");
  var yes1;
  try {
    yes1 = makeR(1);
  }
  writeln(yes1);

  writeln("yes2");
  var yes2:R;
  try! {
    yes2 = makeR(2);
  }
  writeln(yes2);

  writeln("yes3");
  var yes3:R;
  try {
    yes3 = makeR(1);
  } catch e {
    writeln(e);
  }
  writeln(yes3);

  writeln("yes4");
  var yes4;
  try {
    yes4 = makeR(1);
  } catch e {
    writeln(e);
  }
  writeln(yes4);
  
  writeln("no1");
  var no1:R;
  try {
  } catch e {
    no1 = makeR(1);
    writeln(e);
  }
  writeln(no1);
}
try! test();
