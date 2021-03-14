use IO;

// test functions end with "EOB" (a key variable expires end-of-block)
// or "LM" (a key variable expires at last mention)
// or neither (if it's more complex)

class C { var x: int; }

record ComplexR {
  var owns: bool;
  var c: unmanaged C;
  proc init(x: int) {
    this.owns = true;
    this.c = new unmanaged C(x);
  }
  proc init(other: ComplexR) {
    this.owns = false;
    this.c = other.c;
  }
  proc init=(other: ComplexR) {
    this.owns = false;
    this.c = other.c;
  }
  proc deinit() {
    if owns then delete c;
  }
}
proc =(ref lhs: ComplexR, rhs: ComplexR) lifetime lhs=rhs {
  if lhs.owns then
    delete lhs.c;
  lhs.owns = false;
  lhs.c = rhs.c;
}

proc rCopyInitEOB() {
  var a = new ComplexR(1);
  var b = a;
  writeln(a);
  // point 1
  writeln(b);
  // point 2
}
rCopyInitEOB();

proc rCopyInitIshEOB() {
  var a = new ComplexR(1);
  var b = new ComplexR(a);
  writeln(a);
  // point 1
  writeln(b);
  // point 2
}
rCopyInitIshEOB();


proc rAssignEOB() {
  var a = new ComplexR(1);
  var b = new ComplexR(2);
  b = a;
  writeln(a);
  // point 1
  writeln(b);
  // point 2
}
rAssignEOB();

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

proc cInitOtherEOB() {
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
  var a = new ComplexC(1);
  var b = makeBorrow(a);
  writeln(a);
  // point 1
  writeln(b);
  // point 2
}
cMakeBorrowEOB();
