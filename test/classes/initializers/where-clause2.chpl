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

var ownF1 = new owned Foo(13);
var f1 = ownF1.borrow();
var ownF2 = new owned Foo(3.4);
var f2 = ownF2.borrow();
writeln(f1);
writeln(f2);
