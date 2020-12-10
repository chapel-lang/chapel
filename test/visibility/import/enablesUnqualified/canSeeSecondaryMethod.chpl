module OuterModule {
  import secondaryMethod.Foo;
  import this.Extender.Foo;
  // Verifies that you can define a method in a module outside of the original
  // type definition, and by specifying that type in your import list, access
  // it.

  var a = new owned Foo(7);
  writeln(a.innerMethod(3)); // Should be 21
  writeln(a.newMethod(1)); // Should be 8

  module Extender {
    import secondaryMethod.Foo;

    proc Foo.newMethod(z: int) {
      return field + z;
    }

    proc unrelated() {
      writeln("I'm unrelated to Foo!");
    }
  }
}
