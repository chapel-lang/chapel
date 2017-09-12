class Foo {
  param p;

  proc init() {
    p = 2;
    super.init();
  }
}

class Bar : Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
    p = 6;
    super.init();
  }
}

var bar = new Bar(4);
writeln(bar.type: string);
writeln(bar);
delete bar;
