// This test verifies that using the 'only' keyword blocks symbols not specified
// by it.

module A {
  proc foo() {
    writeln("I should not have been found");
  }

  var bar = -13.4;
}

module M {
  use A only bar;

  proc main() {
    foo();
  }
}
