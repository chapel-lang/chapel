module OuterModule {
  use secondaryMethod only Foo;
  use Extender only Foo;
  // Verifies that you can define a method in a module outside of the original
  // type definition, and by specifying that type in your 'only' list, access
  // it.

  var a = new owned Foo(7);
  writeln(a.innerMethod(3)); // Should be 21
  writeln(a.newMethod(1)); // Should be 8

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
