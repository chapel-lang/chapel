class Foo {
  type t;

  proc init() {
    t = bool;

  }
}

class Bar : Foo(?) {
  var x: int;

  proc init(xVal) {
    x = xVal;
    t = real;

  }
}

var bar = new Bar(4);
var b = bar.borrow();
writeln(b.type: string);
writeln(b);
