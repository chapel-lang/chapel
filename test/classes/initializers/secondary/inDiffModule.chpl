// Verified the behavior of types with only secondary initializers when the
// initializer is defined in a module outside of the module where the type is
// originally defined.
module A {
  class Foo {
    var x: int;
    var y = false;
  }

  proc main() {
    var f = new owned Foo(10);
    // Should only use the default constructor (or default initializer when we
    // create those).
    writeln(f); // expect 10, false

  }
}

module B {
  use A;

  proc Foo.init(xVal) {
    writeln("In secondary initializer of class Foo");
    x = xVal;
    y = xVal > 5;
  }
}
