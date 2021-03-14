// Verifies that you can mix modules and enums in the same use statement without
// issue
module Other {
  var d = 16;
}

module M {
  enum foo {a, b, c};

  use Other, foo;

  proc main() {
    writeln(a, " ", b, " ", c);
    writeln(d);
  }
}
