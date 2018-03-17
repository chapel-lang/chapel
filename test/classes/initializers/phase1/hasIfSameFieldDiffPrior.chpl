class Foo {
  var x: int;
  var y: bool;

  proc init(xVal) {
    if (xVal < 5) {
      y = true;
    } else {
      x = xVal;
      y = false;
    }
  }
}

var x = new Foo(4);
var y = new Foo(6);
writeln(x);
writeln(y);
delete x, y;
