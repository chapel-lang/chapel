// Exercises using a where clause when the generic var arguments would otherwise
// make the signatures appear identical.
record Foo {
  var x: int;

  proc init(xVal) where (isInt(xVal)) {
    x = xVal;
  }

  proc init(realVal) where (isReal(realVal)) {
    x = floor(realVal):int;
  }
}

var f1 = new Foo(13);
var f2 = new Foo(3.4);
writeln(f1);
writeln(f2);
