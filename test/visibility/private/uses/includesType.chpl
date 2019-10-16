module Inner {
  class Foo {
    var a = 1;

    proc methodB(x: int) {
      return a + x;
    }
  }

  proc Foo.other() {
    writeln("I'm a secondary on Foo!");
  }
}

module User {
  private use Inner;

  // Verifies that making a use private doesn't interfere with the availability
  // of types defined in that use to the scope which uses it.
  proc main() {
    var foo = new Foo(11);
    writeln(foo.methodB(4));
    foo.other();
  }
}
