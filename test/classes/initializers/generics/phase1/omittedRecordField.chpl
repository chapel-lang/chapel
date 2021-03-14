// Verifies that a more complicated field still gets an appropriate default
// value
class Foo {
  var a: Bar;
  param b: int;

  proc init(param bVal: int) {
    b = bVal;
  }
}

record Bar {
  var x: int;
  var y = 17;
}

var testit: borrowed Foo(11) = new borrowed Foo(11);
writeln(testit);
