// Verify the behavior of types with only secondary initializers when the
// initializer is defined in the same module as the one where the type is
// originally defined.
class Foo {
  var x: int;
  var y = false;
}

proc Foo.init(xVal) {
  writeln("In secondary initializer of class Foo");
  x = xVal;
  y = xVal > 5;
}

proc main() {
  var f = new unmanaged Foo(10);
  // Should use the secondary initializer defined in this module
  // Additionally, we expect the default constructor/initializer to never be
  // created or used. (Do we?)
  writeln(f); // expect 10, true
  delete f;
}
