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

var bar = new Bar(4);
writeln(bar.type: string);
writeln(bar);
delete bar;
