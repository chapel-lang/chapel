use IO;

// test functions end with "EOB" (a key variable expires end-of-block)
// or "LM" (a key variable expires at last mention)
// or neither (if it's more complex)

record R { proc deinit() { } }
class C { var x: int; }

proc setit(ref lhs, rhs) lifetime lhs < rhs {
  lhs = rhs;
}

proc refExample1EOB() {
  var rec = new R();
  ref rf  = rec;
}
refExample1EOB();

proc refExample2EOB() {
  var rec = new R();
  const ref rf  = rec;
}
refExample2EOB();

proc borrowExample1EOB() {
  var c = new owned C();
  var b = c.borrow();
}
borrowExample1EOB();

proc borrowExample2EOB() {
  var c = new owned C();
  var b:borrowed C = c;
}
borrowExample2EOB();

proc borrowExample3EOB() {
  var c = new owned C();
  var b:borrowed C? = c;
}
borrowExample3EOB();

proc borrowExample4EOB() {
  var c = new owned C();
  var b:borrowed C?;
  b = c;
}
borrowExample4EOB();

proc borrowExample5EOB() {
  var c = new owned C();
  var b:borrowed C?;
  setit(b, c);
}
borrowExample5EOB();

proc borrowExample6EOB() {
  var c = new owned C();
  var b:borrowed C?;
  setit(b, c.borrow());
}
borrowExample6EOB();

proc acceptRecord(a) {
}

proc recordExample1LM() {
  var rec = new R();
  acceptRecord(rec);
}
recordExample1LM();

proc recordExample2LM() {
  var rec:R;
  acceptRecord(rec);
}
recordExample2LM();

proc makeR() {
  return new R();
}
proc recordExample3LM() {
  var rec = makeR();
  acceptRecord(rec);
}
recordExample3LM();


proc ioExample0LM() {
  var tmp = opentmp();
}
ioExample0LM();

proc ioExample1LM() {
  var tmp = opentmp();

  var A = [1,2,3,4];
  var B:[A.domain] int;

  tmp.writer().write(A);
  tmp.reader().read(B);
}
ioExample1LM();

proc ownedExample1EOB() {
  var b: borrowed C = (new owned C(1)).borrow();
  // point 1
  writeln(b);
  writeln();
  // point 2
}
ownedExample1EOB();

proc ownedExample2EOB() {
  var b: borrowed C?;
  b = (new owned C(1)).borrow();
  // point 1
  writeln(b);
  writeln();
  // point 2
}
ownedExample2EOB();

proc ownedExample3EOB() {
  var b: borrowed C?;
  setit(b, (new owned C(1)).borrow());
  // point 1
  writeln(b);
  writeln();
  // point 2
}
ownedExample3EOB();

proc ownedExample4LM() {
  var x = new owned C(1);
  var y = x; // ownership transfer to y
  // end of statement mentioning x
}
ownedExample4LM();

class Wrapper { var x; }
proc f() {
  var a: [1..100] int;
  forall i in a.domain {
    a[i] = i;
  }
  return new Wrapper(a);
}
proc arrayExample1LM() {
  var x = f();
}
arrayExample1LM();

proc sliceExample1EOB() {
  var A:[1..100] int;
  ref slice = A[1..10];
  // point 1
  writeln(slice);
  writeln();
  // point 2
}
sliceExample1EOB();

proc makeArray() {
  var A:[1..100] int;
  return A;
}
proc sliceExample2EOB() {
  ref slice = makeArray()[1..10];
  // point 1
  writeln(slice);
  writeln();
  // point 2
}
sliceExample2EOB();

proc innerFnExample1() {
  var c = new C(1);
  var b: borrowed C?;
  proc inner(arg: borrowed C) lifetime b < arg {
    b = arg;
  }
  inner(c);
  // point 1
  writeln(b);
  // point 2
}
innerFnExample1();

proc sharedExample1LMEOB() {
  var c = new shared C();
  var d = new shared C();
  setit(c, d);
}
sharedExample1LMEOB();

proc sharedExample2LM() {
  var c = new shared C();
  var d = new shared C();
  c = d;
}
sharedExample2LM();

proc sharedExample3LM() {
  var c = new shared C();
  var d = c;
}
sharedExample3LM();
