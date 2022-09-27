class Foo {
  var x: int;
  var y: bool;

  proc init(xVal) {
    if (xVal < 5) {
      x = xVal;
      y = true;
    } else {
      x = xVal;
    }
  }
}

var x = (new owned Foo(4)).borrow();
var y = (new owned Foo(6)).borrow();
writeln(x);
writeln(y);
