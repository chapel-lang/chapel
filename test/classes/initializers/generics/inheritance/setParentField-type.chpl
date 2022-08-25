class Foo {
  type t;

  proc init() {
    t = bool;

  }
}

class Bar : Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
    t = real;

  }
}

var bar = (new owned Bar(4)).borrow();
writeln(bar.type: string);
writeln(bar);
