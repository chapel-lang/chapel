// Verifies that specifying a symbol that doesn't exist in an only list fails
// as expected.
module Foo {
  var a: bool;
  var b = 16.5;

  proc c() {
    writeln("Just a placeholder");
  }
}

module M {
  use Foo only d;

  proc main() {
    writeln(d);
  }
}
