module OuterModule {
  use secondaryMethodRecord only Foo;
  use Extender only Foo;
  // Verifies that you can define a method in a module outside of the original
  // type definition, and by specifying that type in your 'only' list, access
  // it.

  var a = new Foo(7);
  var b = new Foo(2);
  writeln(a.innerMethod(3)); // Should be 21
  writeln(a + b);            // Should be (field = 9)

  module Extender {
    use secondaryMethodRecord only Foo;

    operator Foo.+(lhs: Foo, rhs: Foo) {
      writeln("In operator overload");
      return new Foo(lhs.field + rhs.field);
    }

    proc unrelated() {
      writeln("I'm unrelated to Foo!");
    }
  }
}
