// Verifies that a more complicated field still gets an appropriate default
// value
class Foo {
  var a: Bar;
  var b: int;

  proc init(bVal: int) {
    b = bVal;
  }
}

record Bar {
  var x: int;
  var y = 17;
}

var testit: borrowed Foo = new borrowed Foo(11);
writeln(testit);
