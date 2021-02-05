module OuterModule {
  use secondaryMethod only Foo;
  use Extender except Foo;
  // Verifies that you can define a method in a module outside of the original
  // type definition, and by specifying that type in your 'except' list, exclude
  // it.

  var a = new owned Foo(7);
  writeln(a.innerMethod(3)); // Should be 21
  writeln(a.newMethod(1)); // Should fail to compile

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
