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

var bar = new borrowed Bar(4);
writeln(bar.type: string);
writeln(bar);
