// Exercises using a where clause when the generic var arguments would otherwise
// make the signatures appear identical.
class Foo {
  var x: int;

  proc init(xVal) where (isInt(xVal)) {
    x = xVal;
  }

  proc init(realVal) where (isReal(realVal)) {
    x = floor(realVal):int;
  }
}

var f1 = (new owned Foo(13)).borrow();
var f2 = (new owned Foo(3.4)).borrow();
writeln(f1);
writeln(f2);
