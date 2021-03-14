// This test verifies that multiple except chains don't block more than they
// should

module A {
  public use B except bar;

  var foo: uint;

  var baz = 5 + 4i;
}

module B {
  var bar = true;

  var mop = 15;
}

module M {
  public use A except foo;

  proc main() {
    writeln(baz); // Visible from A
    writeln(mop); // Visible from A's nested use of B
  }
}
