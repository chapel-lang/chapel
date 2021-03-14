// Exercises using a where clause to eliminate other calls
class Foo {
  var x: int;

  proc init(xVal) where (isInt(xVal)) {
    x = xVal;

  }
}

var f1 = new unmanaged Foo(13);
var f2 = new unmanaged Foo(3.4);
writeln(f1);
writeln(f2);
delete f1;
delete f2;
