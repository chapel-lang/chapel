pragma "safe"
module SaveBorrowInCollection {

class MyClass {
  var x:int;
}

var A:[1..1] borrowed MyClass?;

proc setA(arg: borrowed MyClass) {
  A[1] = arg;
}

var global: borrowed MyClass?;

proc error1(arg: borrowed MyClass) {
  global = arg;
}

proc error2(arg: borrowed MyClass) {
  var x = arg;
  global = x;
}

proc error3(arg: borrowed MyClass) {
  ref x = global;
  x = arg;
}

record BorrowedThing {
  var b: borrowed MyClass?;
}

var globalB: BorrowedThing;

proc error4(arg: borrowed MyClass) {
  globalB.b = arg;
}

proc BorrowedThing.error5(arg: borrowed MyClass) {
  this.b = arg;
}

proc call_error5(arg: borrowed MyClass) {
  globalB.error5(arg);
}

var globalA: [1..1] borrowed MyClass?;

proc error6(arg: borrowed MyClass) {
  globalA[1] = arg;
}

proc test() {
  var x = new owned MyClass(1);
  var b = x.borrow();
  error1(b);
  error2(b);
  error3(b);
  error4(b);
  call_error5(b);
  error6(b);
}
test();

}
