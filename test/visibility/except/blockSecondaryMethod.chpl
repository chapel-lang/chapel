module OuterModule {
  use secondaryMethod except other;
  use Extender except Foo;
  // Verifies that methods defined outside of the type can be squashed by an
  // 'except' on the type for which they are defined.

  var a = new borrowed Foo(7);
  writeln(a.innerMethod(3)); // Should be 21
  writeln(a.newMethod(1)); // Should fail to resolve

  module Extender {
    use secondaryMethod only Foo;

    proc Foo.newMethod(z: int) {
      return field + z;
    }

    proc unrelated() {
      writeln("I'm unrelated to Foo!");
    }
  }
}
