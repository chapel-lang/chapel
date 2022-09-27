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

var x = (new owned Foo(4)).borrow();
var y = (new owned Foo(6)).borrow();
writeln(x);
writeln(y);
