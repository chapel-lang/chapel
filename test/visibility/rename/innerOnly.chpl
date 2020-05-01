module A {
  public use B only bar;
  var foo = false;
}

module B {
  var bar = 92;
}

module M {
  use A only bar as baz;

  proc main() {
    writeln(baz); // Should refer to B.bar
  }
}
