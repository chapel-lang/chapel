// Verifies that using the 'only' keyword allows us to find the symbol specified
// by it.
module A {
  var bar = true;

  var baz = 14;
}

module M {
  use A only bar;

  proc main() {
    writeln(bar);
  }
}
