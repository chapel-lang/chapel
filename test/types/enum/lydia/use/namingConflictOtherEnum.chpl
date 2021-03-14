// Verifies that a naming conflict occurs when an enum component name is shared
// by one of another enum's component names.
module M {
  enum foo {a, b, c};

  enum bar {a, d, e};

  use foo;
  use bar;

  proc main() {
    writeln(a); // Can't decide between foo.a and bar.a
  }
}
