// Verify the behavior of types with both primary and secondary initializers
// when the secondary initializer is defined in the same module as the one
// where the type is originally defined.
class Foo {
  var x: int;
  var y = false;

  proc init(yVal: bool) {
    writeln("In primary initializer of class Foo");
    x = 3;
    y = yVal;
    super.init();
  }
}

proc Foo.init(xVal: int) {
  writeln("In secondary initializer of class Foo");
  x = xVal;
  y = xVal > 5;
  super.init();
}

proc main() {
  var f = new Foo(10);
  // Should use the secondary initializer defined in this module
  writeln(f); // expect 10, true
  var f2 = new Foo(false);
  writeln(f2); // expect 3, false
}
