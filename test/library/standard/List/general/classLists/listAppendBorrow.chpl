pragma "safe"
module SaveBorrowInList {

private use List;

class foo {
  var x: int = 0;
}

var global: borrowed foo?;

var globalArr1: [1..1] borrowed foo?;
var globalLst1: list(borrowed foo?) = globalArr1;

// Should emit a lifetime error.
proc error7(arg: borrowed foo) {
  globalLst1.append(arg);
}

proc ok() {
  globalLst1.append(global);
}

proc main() {
  var x = new owned foo(1);
  var b = x.borrow();
  error7(b);
}

}
