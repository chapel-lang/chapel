pragma "safe"
module SaveBorrowInList {

private use List;

class foo {
  var x: int = 0;
}

var arr1: [1..1] borrowed foo?;
var lst1: list(borrowed foo?) = arr1;

proc setArr1(arg: borrowed foo) {
  lst1[1] = arg;
}

var global: borrowed foo?;

var globalArr1: [1..1] borrowed foo?;
var globalLst1: list(borrowed foo?) = globalArr1;

proc error7(arg: borrowed foo) {
  globalLst1.append(arg);
}

proc main() {
  var x = new owned foo(1);
  var b = x.borrow();
  error7(b);
}

}
