// This verifies that symbols further in the use chain are blocked by the 'only'
// when they should be.
module A {
  use B;

  var foo = 83;
}

module B {
  proc bar() {
    writeln("Wheeeeeee");
  }
}

module M {
  use A only foo;

  proc main() {
    bar(); // Should fail
  }
}
