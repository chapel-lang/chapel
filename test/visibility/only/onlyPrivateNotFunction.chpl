// Verifies that specifying a private symbol in an only list fails as expected.
module Foo {
  var a: bool;
  var b = 16.5;

  proc c() {
    writeln("Just a placeholder");
  }

  private var d = false;
}

module M {
  use Foo only d;

  proc main() {
    writeln(d);
  }
}
