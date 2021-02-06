module OuterModule {
  import secondaryMethod.Foo;
  import this.Extender.Foo;
  // Verifies that you can define a method in a module outside of the original
  // type definition, and by specifying that type in your import list, access
  // it.

  var a = new owned Foo(7);
  writeln(a.innerMethod(3)); // Should be 21
  writeln(Foo.newMethod(1)); // Should be 1

  module Extender {
    import secondaryMethod.Foo;

    proc type Foo.newMethod(z: int) {
      return z;
    }

    proc unrelated() {
      writeln("I'm unrelated to Foo!");
    }
  }
}
