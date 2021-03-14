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

var bar = new borrowed Bar(4);
writeln(bar.type: string);
writeln(bar);
