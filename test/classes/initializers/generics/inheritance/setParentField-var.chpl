class Foo {
  var v;

  proc init() {
    v = 2;
    super.init();
  }
}

class Bar : Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
    v = x + 2;
    super.init();
  }
}

var bar = new Bar(4);
writeln(bar);
delete bar;
