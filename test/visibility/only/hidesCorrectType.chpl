// This test verifies that using the 'only' keyword blocks symbols not specified
// by it.

module A {
  type foo = complex;

  var bar = -13.4;
}

module M {
  use A only bar;

  proc main() {
    var a: foo;
    writeln(a);
  }
}
