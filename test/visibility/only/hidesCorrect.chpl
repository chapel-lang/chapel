// This test verifies that using the 'only' keyword blocks symbols not specified
// by it.

module A {
  var foo: complex;

  var bar = -13.4;
}

module M {
  use A only bar;

  proc main() {
    writeln(foo);
  }
}
