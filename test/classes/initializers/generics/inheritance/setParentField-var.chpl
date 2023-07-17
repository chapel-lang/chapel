class Foo {
  var v;

  proc init() {
    v = 2;

  }
}

class Bar : Foo {
  var x: int;

  proc init(xVal) {
    // It is OK to set a parent field in phase 1
    x = xVal;
    v = x + 2;

  }
}

var ownBar = new owned Bar(4);
var bar = ownBar.borrow();
writeln(bar);
