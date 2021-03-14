// This file records cases where the analysis allows early deinit
// but that are arguably suprising, or at least where the change
// is noticeable.

class C { var x: int; }

record ComplexR {
  var owns: bool;
  var c: unmanaged C;
  proc init(x: int) {
    this.owns = true;
    this.c = new unmanaged C(x);
  }
  proc init=(other: ComplexR) {
    this.owns = false;
    this.c = other.c;
  }
  proc deinit() {
    if owns then delete c;
  }
}
proc =(ref lhs: ComplexR, rhs: ComplexR) {
  if lhs.owns then
    delete lhs.c;
  lhs.owns = false;
  lhs.c = rhs.c;
}

proc rAssign() {
  var a = new ComplexR(1);
  var b = new ComplexR(2);
  b = a;
  writeln(a);
  // point 1
  writeln(b);
  // point 2
}
rAssign();

proc rAssignFields() {
  var a = new ComplexR(1);
  var b = new ComplexR(2);
  delete b.c;
  b.owns = false;
  b.c = a.c;
  writeln(a);
  // point 1
  writeln(b);
  // point 2
}
rAssignFields();

proc makeR() {
  return new ComplexR(1);
}


var global: unmanaged C = new unmanaged C(1);

proc captureA(arg: ComplexR) {
  global = arg.c;  // unmanaged, so no lifetime error
}

proc captureB(ref lhs: unmanaged C?, rhs: ComplexR) {
  lhs = rhs.c; // unmanaged, so no lifetime error
}

proc testRCaptures() {
  var u: unmanaged C?;

  var tmp = makeR();
  captureA(tmp);
  captureB(u, tmp);
  // point 1 - tmp deleted here?

  writeln("The End ", u, global);

  // adding the following line would keep tmp from being deinited at point 1
  //   tmp;

  // point 2
}
testRCaptures();

class ComplexC {
  var owns: bool;
  var c: unmanaged C;
  proc init(x: int) {
    this.owns = true;
    this.c = new unmanaged C(x);
  }
  proc init(other: borrowed ComplexC) {
    this.owns = false;
    this.c = other.c;
  }
  proc deinit() {
    if owns then delete c;
  }
}

proc cAssignFieldEOB() {
  var a = new ComplexR(1);
  var b = new ComplexR(2);
  b.owns = false;
  b.c = a.c;
  writeln(a);
  // point 1
  writeln(b);
  // point 2
}
cAssignFieldEOB();

record R {
  var c: unmanaged C?;
  proc init(arg: unmanaged C?) {
    this.c = arg;
  }
  proc init=(other:R) {
    this.c = new unmanaged C(other.c!.x);
  }
}
var globr = new R(new unmanaged C(1));
proc aliasesGlobal1() {
  var r = new R(globr.c);
  foo(r);
}
proc foo(in arg: R) {
  globr.c!.x = 2;
  writeln(arg.c!.x); // does it print out 1 or 2?
}
aliasesGlobal1();

proc aliasesGlobal2() {
  var r = new R(globr.c);
  var y = r;
  globr.c!.x = 2;
  writeln(y.c!.x); // does it print out 1 or 2?
}
