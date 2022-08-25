class Foo {
  param p;

  proc init() {
    p = 2;

  }
}

class Bar : Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
    p = 6;

  }
}

var bar = (new owned Bar(4)).borrow();
writeln(bar.type: string);
writeln(bar);
