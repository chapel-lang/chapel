// Verified the behavior of types with only secondary initializers when the
// initializer is defined in a module outside of the module where the type is
// originally defined.

// Variation on inDiffModule.chpl where the modules are defined in a different
// order
module B {
  use A;

  proc Foo.init(xVal) {
    writeln("In secondary initializer of class Foo");
    x = xVal;
    y = xVal > 5;
  }
}

module A {
  class Foo {
    var x: int;
    var y = false;
  }

  proc main() {
    var f = new unmanaged Foo(10);
    // Should only use the default constructor (or default initializer when we
    // create those).
    writeln(f); // expect 10, false
    delete f;
  }
}
