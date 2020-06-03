// records/classes using owned + borrowed and not unmanaged
// checking the cases from challenges.chpl

class C { var x: int; }

record ComplexR {
  var own: owned C?;
  var bor: borrowed C?;
  proc value(): borrowed C? {
    if own != nil then return own.borrow();
    else return bor;
  }
  proc init(x: int) {
    this.own = new owned C(x);
  }
  proc init(other: ComplexR) {
    this.own = nil;
    this.bor = other.value();
  }
  proc init=(other: ComplexR) {
    this.own = nil;
    this.bor = other.value();
  }
}
proc =(ref lhs: ComplexR, rhs: ComplexR) lifetime lhs=rhs {
  lhs.own = nil;
  lhs.bor = rhs.value();
}

proc rCopyInitEOB() {
  writeln("rCopyInitEOB");
  var a = new ComplexR(1);
  var b = a;
  writeln(a);
  // point 1
  writeln(b);
  // point 2
}
rCopyInitEOB();

proc rCopyInitIshEOB() {
  writeln("rCopyInitIshEOB");
  var a = new ComplexR(1);
  var b = new ComplexR(a);
  writeln(a);
  // point 1
  writeln(b);
  // point 2
}
rCopyInitIshEOB();


proc rAssignEOB() {
  writeln("rAssignEOB");
  var a = new ComplexR(1);
  var b = new ComplexR(2);
  b = a;
  writeln(a);
  // point 1
  writeln(b);
  // point 2
}
rAssignEOB();

proc rAssignFieldsEOB() {
  writeln("rAssignFieldsEOB");
  var a = new ComplexR(1);
  var b = new ComplexR(2);
  b.own = nil;
  b.bor = a.value();
  writeln(a);
  // point 1
  writeln(b);
  // point 2
}
rAssignFieldsEOB();

proc makeBorrow(arg: ComplexR) {
  var ret = arg;
  return ret;
}

proc rMakeBorrowEOB() {
  var a = new ComplexR(1);
  var b = makeBorrow(a);
  writeln(a);
  // point 1
  writeln(b);
  // point 2
}
rMakeBorrowEOB();


proc makeR() {
  return new ComplexR(1);
}

var global: borrowed C? = nil;

// The following two functions generate lifetime
// errors if the clauses are not present
proc captureA(arg: ComplexR) lifetime global = arg {
  global = arg.value();
}

proc captureB(ref lhs: borrowed C?, rhs: ComplexR) lifetime lhs = rhs {
  lhs = rhs.value();
}

proc testRCapturesEOB() {
  writeln("testRCapturesEOB");

  var u: borrowed C?;

  var r = makeR();
  captureA(r);
  captureB(u, r);
  // point 1 - r deleted here?

  writeln("The End ", u, global);

  // point 2
}
testRCapturesEOB();

proc testRCapturesaEOB() {
  writeln("testRCapturesaEOB");

  var r = makeR();
  captureA(r);
  // point 1 - r deleted here?

  writeln("The End ", global);

  // point 2
}
testRCapturesaEOB();

proc testRCapturesbEOB() {
  writeln("testRCapturesrbEOB");

  var u: borrowed C?;

  var r = makeR();
  captureB(u, r);
  // point 1 - r deleted here?

  writeln("The End ", u);

  // point 2
}
testRCapturesbEOB();


class ComplexC {
  var own: owned C?;
  var bor: borrowed C?;
  proc value(): borrowed C? {
    if own != nil then return own.borrow();
    else return bor;
  }
  proc init(x: int) {
    this.own = new owned C(x);
  }
  proc init(other: borrowed ComplexC) {
    this.own = nil;
    this.bor = other.value();
  }
}

proc cInitOtherEOB() {
  writeln("cInitOtherEOB");
  var a = new ComplexC(1);
  var b = new ComplexC(a);
  writeln(a);
  // point 1
  writeln(b);
  // point 2
}
cInitOtherEOB();

proc makeBorrow(arg: ComplexC) {
  var ret = new ComplexC(arg);
  return ret;
}

proc cMakeBorrowEOB() {
  writeln("cMakeBorrowEOB");
  var a = new ComplexC(1);
  var b = makeBorrow(a);
  writeln(a);
  // point 1
  writeln(b);
  // point 2
}
cMakeBorrowEOB();

proc cAssignFieldEOB() {
  writeln("cAssignFieldEOB");
  var a = new ComplexR(1);
  var b = new ComplexR(2);
  b.own = nil;
  b.bor = a.value();
  writeln(a);
  // point 1
  writeln(b);
  // point 2
}
cAssignFieldEOB();
