module A {
  use B except bar;

  var foo = 92;
}

module B {
  var bar = 19.0;

  var bleep = false;
}

module M {
  use A only bar as baz;
  // Should fail, A's use of B hides bar.

  proc main() {
    writeln(baz);
  }
}
