module A {
  public use B only foo as bar;
}

module B {
  var foo = 92;
}

module M {
  use A only bar as baz;

  proc main() {
    writeln(baz); // Should refer to B.foo
  }
}
