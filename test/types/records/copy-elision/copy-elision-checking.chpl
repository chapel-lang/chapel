config param printInitDeinit = true;

class MyClass { var x: int; }

record RR {
  var x: int = 1;
  var c: shared MyClass = new shared MyClass(x);
}
proc RR.init(arg: int) {
  this.x = arg;
  this.c = new shared MyClass(arg);
}
proc RR.init=(other: RR) {
  this.x = other.x;
  this.c = new shared MyClass(other.c.x);
}
proc =(ref lhs: RR, rhs: RR) {
  lhs.x = rhs.x;
  lhs.c = new shared MyClass(rhs.c.x);
}

proc test1() {
  var x = new RR(1);
  ref r = x;
  var y = x;
  r.x = 2;   // yeah but x is dead at this point
  r.c.x = 2; // should not modify y's value
  writeln(y);
}
test1();

proc test2() {
  var x = new RR(1);
  ref r = x;
  var y = x;
  r.c.x = 2; // x is dead at this point
  writeln(y);
}
test2();

proc acceptIn(in arg) { }

proc test10() {
  var x = new RR(1);
  ref rx = x;
  acceptIn(x); // moves value to formal, function takes ownership, may deinit
  writeln(rx); // use-after free
}
test10();

proc test10a() {
  var x = new RR(1);
  ref rx = x;
  var y = x; // moves value to formal, function takes ownership, may deinit
  writeln(rx); // use-after free
}
test10a();

proc test11() {
  var x = new shared MyClass(1);
  var y = x.borrow();
  acceptIn(x); // moves, frees
  writeln(y);
}
test11();

proc test11a() {
  var x = new shared MyClass(1);
  var y = x.borrow();
  var z = x; // moves
  writeln(y);
}
test11a();

proc test12() {
  var x = new shared MyClass?(1);
  var y = x.borrow();
  acceptIn(x); // moves, frees
  writeln(y!);
}
test12();

proc test12a() {
  var x = new shared MyClass?(1);
  var y = x.borrow();
  var z = x; // moves
  writeln(y!);
}
test12a();

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

proc set1f(ref arg: R) ref {
  arg.set1();
  return arg;
}

proc concatenate(a: R, b: R): string {
  return a.toString() + " " + b.toString();
}

proc concatenate(a: R, b: string): string {
  return a.toString() + " " + b;
}

proc concatenate(a: R, b: borrowed C): string {
  return a.toString() + " " + b.xx:string;
}

proc concatenate(a: R, b: int): string {
  return a.toString() + " " + b:string;
}

proc copy(in arg) {
  return arg;
}

proc acceptTwoIn(in a, in b) {
  return concatenate(a, b);
}

proc test2a() {
  writeln("test2a");
  var x = new R(0);
  ref rx = x;
  writeln("test2a.a");
  var y = x;
  writeln("test2a.b");
  writeln(concatenate(rx.set1(), y));
}
test2a();

proc test2b() {
  writeln("test2b");
  var x = new R(0);
  ref rx = x;
  writeln("test2b.a");
  var y = x;
  writeln("test2b.b");
  return concatenate(rx.set1(), y);
}
{ writeln(test2b()); }

proc test2c() {
  writeln("test2c");
  var x = new R(0);
  ref rx = x;
  writeln("test2c.a");
  var y = x;
  writeln("test2c.b");
  writeln(concatenate(set1f(rx), y));
}
test2c();

proc test2d() {
  writeln("test2d");
  var x = new R(0);
  ref rx = x;
  writeln("test2d.a");
  var y = x;
  writeln("test2d.b");
  return concatenate(set1f(rx), y);
}
{ writeln(test2d()); }


class CD {
  var xx: int = 0;
  proc init(arg: int) {
    this.xx = arg;
    if printInitDeinit then writeln("C.init ", arg);
  }
  proc deinit() {
    if printInitDeinit then writeln("C.deinit ", xx);
  }
}

record RD {
  var x: int = 0;
  var ptr: shared CD = new shared C(0);
  proc init() {
    this.x = 0;
    this.ptr = new shared CD(0);
    if printInitDeinit then writeln("init (default)");
  }
  proc init(arg:int) {
    this.x = arg;
    this.ptr = new shared CD(arg);
    if printInitDeinit then writeln("init ", arg, " ", arg);
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

proc set1f(ref arg: RD) ref {
  arg.set1();
  return arg;
}

proc concatenate(a: RD, b: RD): string {
  return a.toString() + " " + b.toString();
}

proc concatenate(a: RD, b: string): string {
  return a.toString() + " " + b;
}

proc concatenate(a: RD, b: borrowed CD): string {
  return a.toString() + " " + b.xx:string;
}

proc concatenate(a: RD, b: int): string {
  return a.toString() + " " + b:string;
}

proc test2aD() {
  writeln("test2aD");
  var x = new RD(0);
  ref rx = x;
  writeln("test2aD.a");
  var y = x;
  writeln("test2aD.b");
  writeln(concatenate(rx.set1(), y));
}
test2aD();

proc test2bD() {
  writeln("test2bD");
  var x = new RD(0);
  ref rx = x;
  writeln("test2bD.a");
  var y = x;
  writeln("test2bD.b");
  return concatenate(rx.set1(), y);
}
{ writeln(test2bD()); }

proc test2cD() {
  writeln("test2cD");
  var x = new RD(0);
  ref rx = x;
  writeln("test2cD.a");
  var y = x;
  writeln("test2cD.b");
  writeln(concatenate(set1f(rx), y));
}
test2cD();

proc test2dD() {
  writeln("test2dD");
  var x = new RD(0);
  ref rx = x;
  writeln("test2d.a");
  var y = x;
  writeln("test2d.b");
  return concatenate(set1f(rx), y);
}
{ writeln(test2dD()); }

