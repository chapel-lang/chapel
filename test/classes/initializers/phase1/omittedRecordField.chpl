// Verifies that a more complicated field still gets an appropriate default
// value
class Foo {
  var a: Bar;
  var b: int;

  proc init(bVal: int) {
    b = bVal;
    super.init();
  }
}

record Bar {
  var x: int;
  var y = 17;
}

var testit: Foo = new Foo(11);
writeln(testit);
