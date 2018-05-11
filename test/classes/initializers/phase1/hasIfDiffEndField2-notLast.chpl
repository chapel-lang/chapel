class Foo {
  var x: int;
  var y: bool;
  var z: int;

  proc init(xVal) {
    if (xVal < 5) {
      x = xVal;
      y = true;
    } else {
      x = xVal;
    }
  }
}

var x = new owned Foo(4);
var y = new owned Foo(6);
writeln(x);
writeln(y);
