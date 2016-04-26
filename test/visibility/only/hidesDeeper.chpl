// This verifies that symbols further in the use chain are blocked by the 'only'
// when they should be.
module A {
  use B;

  var foo = 83;
}

module B {
  var bar: bool;
}

module M {
  use A only foo;

  proc main() {
    writeln(bar); // Should fail
  }
}
