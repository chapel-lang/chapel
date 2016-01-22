// Verifies that specifying a private symbol in an only list fails as expected.
module Foo {
  var a: bool;
  var b = 16.5;

  proc c() {
    writeln("Just a placeholder");
  }

  private proc d() {
    writeln("Oh hey, you found me.  Thought I hid better than that");
  }
}

module M {
  use Foo only d;

  proc main() {
    d();
  }
}
