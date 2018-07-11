pragma "safe"
module SaveBorrowInCollection {

class MyClass {
  var x:int;
}

var A:[1..1] borrowed MyClass;

proc setA(arg: borrowed MyClass) {
  A[1] = arg;
}

var global: borrowed MyClass;

var globalA: [1..1] borrowed MyClass;

proc error7(arg: borrowed MyClass) {
  globalA.push_back(arg);
}

proc test() {
  var x = new owned MyClass(1);
  var b = x.borrow();
  error7(b);
}
test();

}
