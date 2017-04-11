// Verified the behavior of types with only secondary initializers when the
// initializer is defined in a module outside of the module where the type is
// originally defined.
module A {
  class Foo {
    var x: int;
    var y = false;
  }
}

module B {
  use A;

  proc Foo.init(xVal) {
    writeln("In secondary initializer of class Foo");
    x = xVal;
    y = xVal > 5;
    super.init();
  }

  proc main() {
    var f = new Foo(10);
    // Should use the secondary initializer defined in this module
    // Additionally, we expect the default constructor/initializer to not be
    // accessible from this scope. (Do we?)
    writeln(f); // expect 10, true
    delete f;
  }
}
