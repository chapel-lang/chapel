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

var ownX = new owned Foo(4);
var x = ownX.borrow();
var ownY = new owned Foo(6);
var y = ownY.borrow();
writeln(x);
writeln(y);
