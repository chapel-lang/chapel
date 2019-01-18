// Verified the behavior of types with only secondary initializers when the
// initializer is defined in a module outside of the module where the type is
// originally defined.

// Variation on inDiffModule.chpl where the modules are defined in a different
// order
module B {
  use A;

  proc Foo.init(xVal: int) {
    writeln("In secondary initializer of class Foo");
    x = xVal;
    y = xVal > 5;
  }
}

module A {
  class Foo {
    var x: int;
    var y = false;

    proc init(yVal: bool) {
      writeln("In primary initializer of class Foo");
      x = 3;
      y = yVal;
    }
  }

  proc main() {
    var f = new borrowed Foo(true);
    writeln(f); // expect 3, true
  }
}
