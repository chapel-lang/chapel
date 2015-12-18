// This test verifies that multiple except chains don't drop the inner except
// list

module A {
  use B except bar;

  var foo: uint;

  var baz = 5 + 4i;
}

module B {
  var bar = true;

  var mop = 15;
}

module M {
  use A except foo;

  proc main() {
    writeln(bar); // Should fail to resolve
  }
}
