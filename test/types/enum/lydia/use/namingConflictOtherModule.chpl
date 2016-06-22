// Verifies that a naming conflict occurs when an enum component name is shared
// by a name included from a module being used.
module Other {
  var a = 16;
}

module M {
  enum foo {a, b, c};

  use Other;
  use foo;

  proc main() {
    writeln(a); // Can't decide between foo.a and Other.a
  }
}
