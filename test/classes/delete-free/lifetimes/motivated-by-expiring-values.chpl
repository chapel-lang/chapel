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

proc =(ref lhs: ComplexR, rhs: ComplexR) {
  lhs.own = nil;
  lhs.bor = rhs.value();
}

proc rAssignEOB() {
  writeln("rAssignEOB");
  var b = new ComplexR(2);
  {
    var a = new ComplexR(1);
    b = a;
    writeln(a);
  }
  writeln(b);
}
rAssignEOB();

proc makeR() {
  return new ComplexR(1);
}

var global: borrowed C? = nil;

// expecting lifetime error
proc captureA(arg: ComplexR) {
  global = arg.value();
}

// expecting lifetime error
proc captureB(ref lhs: borrowed C?, rhs: ComplexR) {
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


proc testAcapturesTemp() {
  var A:[1..1] borrowed C?;

  A[1] = new C(1);

  writeln(A);
}
testAcapturesTemp();
