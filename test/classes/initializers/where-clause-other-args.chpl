// Exercises using a where clause when not every argument is used in the where
// clause
class Foo {
  var x: int;
  var y: int;

  proc init(param xVal, yVal) where (xVal > 10) {
    x = xVal;
    y = yVal;
  }

  proc init(param xVal) where (xVal < 10) {
    x = xVal;
  }
}

var f1 = new unmanaged Foo(13, 4);
var f2 = new unmanaged Foo(3);
writeln(f1);
writeln(f2);
delete f1;
delete f2;
