private use List;

class foo {
  var x: int = 0;
}

var arr1: [1..1] borrowed foo?;
var lst1: list(borrowed foo?) = arr1;

proc setGlobalLst1(arg: borrowed foo) {
  lst1[1] = arg;
}

proc test() {
  var x = new owned foo(1);
  var b = x.borrow();
  setGlobalLst1(b);
}
test();

