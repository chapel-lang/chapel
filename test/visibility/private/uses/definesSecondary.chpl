module TypeDefiner {
  class Foo {
    var a = 1;

    proc methodB(x: int) {
      return a + x;
    }
  }
}

module SecondaryDefiner {
  public use TypeDefiner;

  proc Foo.other() {
    writeln("I'm a secondary on Foo!");
  }
}

module User {
  private use SecondaryDefiner;

  // Ensures that using a module privately doesn't break secondary methods
  // defined in modules which don't contain the original type definition.
  proc main() {
    var foo: Foo = new Foo(3);
    writeln(foo.methodB(2)); // Should be 5
    foo.other();
  }
}
