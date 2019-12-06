use IO;

record R { proc deinit() { } }
class C { var x: int; }

proc refExample1() {
  var rec = new R();
  ref rf  = rec;
}
refExample1();

proc refExample2() {
  var rec = new R();
  const ref rf  = rec;
}
refExample2();

proc borrowExample1() {
  var c = new owned C();
  var b = c.borrow();
}
borrowExample1();

proc borrowExample2() {
  var c = new owned C();
  var b:borrowed C = c;
}
borrowExample2();

proc borrowExample3() {
  var c = new owned C();
  var b:borrowed C? = c;
}
borrowExample3();

proc acceptRecord(a) {
}

proc recordExample1() {
  var rec = new R();
  acceptRecord(rec);
}
recordExample1();

proc recordExample2() {
  var rec:R;
  acceptRecord(rec);
}
recordExample2();

proc makeR() {
  return new R();
}
proc recordExample3() {
  var rec = makeR();
  acceptRecord(rec);
}
recordExample3();


proc ioExample0() {
  var tmp = opentmp();
}
ioExample0();

proc ioExample1() {
  var tmp = opentmp();

  var A = [1,2,3,4];
  var B:[A.domain] int;

  tmp.writer().write(A); // TODO
  tmp.reader().read(B);
}
ioExample1();

proc ownedExample1() {
  var instance: borrowed C = (new owned C(1)).borrow();
  // point 1
  writeln(instance);
  writeln();
  // point 2
}
ownedExample1();

proc ownedExample2() {
  var x = new owned C(1);
  var y = x; // ownership transfer to y
  // end of statement mentioning x
}
ownedExample2();

class Wrapper { var x; }
proc f() {
  var a: [1..100] int;
  forall i in a.domain {
    a[i] = i;
  }
  return new Wrapper(a);
}
proc arrayExample1() {
  var x = f();
}
arrayExample1();

proc sliceExample1() {
  var A:[1..100] int;
  ref slice = A[1..10];
  // point 1
  writeln(slice);
  writeln();
  // point 2
}
sliceExample1();

proc makeArray() {
  var A:[1..100] int;
  return A;
}
proc sliceExample2() {
  ref slice = makeArray()[1..10];
  // point 1
  writeln(slice);
  writeln();
  // point 2
}
sliceExample2();
